#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <fcntl.h>   // File control definitions
#include <termios.h> // POSIX terminal control definitions
#include <unistd.h>  // UNIX standard function definitions

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class DistancePublisher : public rclcpp::Node {
public:
  DistancePublisher() : Node("distance_publisher") {
    publisher_ = this->create_publisher<std_msgs::msg::Int32>("sensor_distance", 10);
    
    // Open the Serial Port - Match this to your Arduino port (usually /dev/ttyACM0)
    serial_port_ = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);

    if (serial_port_ < 0) {
      RCLCPP_ERROR(this->get_logger(), "Unable to open serial port. Check permissions!");
      return;
    }

    // Configure Serial Settings
    struct termios tty;
    tcgetattr(serial_port_, &tty);    cfsetispeed(&tty, B115200); // Must match Arduino Serial.begin(115200)
    cfsetospeed(&tty, B115200);
    tty.c_lflag |= ICANON;      // Line-by-line mode
    tcsetattr(serial_port_, TCSANOW, &tty);

    // Create a timer to read and publish every 100ms
    timer_ = this->create_wall_timer(100ms, std::bind(&DistancePublisher::timer_callback, this));
  }

  ~DistancePublisher() { close(serial_port_); }

private:
  void timer_callback() {
    char buf[64];
    int n = read(serial_port_, buf, sizeof(buf) - 1);

    if (n > 0) {
      buf[n] = '\0'; // Null-terminate the string
      try {
        auto message = std_msgs::msg::Int32();
        message.data = std::stoi(buf); // Convert string to integer
        publisher_->publish(message);
        RCLCPP_INFO(this->get_logger(), "Published Distance: %d", message.data);
      } catch (...) {
        // Skips malformed lines during serial startup
      }
    }
  }

  int serial_port_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DistancePublisher>());
  rclcpp::shutdown();
  return 0;
}