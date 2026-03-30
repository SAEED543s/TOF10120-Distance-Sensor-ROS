# TOF10120 Distance Sensor with Arduino and ROS

هذا المشروع يقدم حلاً لربط حساس المسافة **TOF10120** بلوحة الأردوينو وإرسال البيانات إلى نظام تشغيل الروبوتات **ROS**.

## المكونات المستخدمة (Hardware)
- Arduino Uno / Nano.
- TOF10120 Laser Distance Sensor.
- Jumper wires.

## المتطلبات البرمجية (Software)
- **Arduino IDE**
- **ROS** (Noetic / Melodic / Humble)
- **rosserial_arduino** package.

## مخطط التوصيل (Wiring)
- **VCC** -> 5V
- **GND** -> GND
- **RX/TX** -> المتصلة بالأردوينو (حسب الكود).

## طريقة التشغيل
1. قم برفع كود الأردوينو الموجود في مجلد `arduino_code`.
2. افتح الـ Terminal في ROS وشغل الأمر التالي لربط السيريال:
   ```bash
   rosrun rosserial_python serial_node.py _port:=/dev/ttyUSB0
