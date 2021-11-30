# hidl服务

## build
```java

```

## push & kill
```java
adb push out/target/product/lemonadep/vendor/bin/hw/com.journeyOS.J007engine.hidl@1.0-service vendor/bin/hw/com.journeyOS.J007engine.hidl@1.0-service
adb push out/target/product/lemonadep/vendor/lib64/com.journeyOS.J007engine.hidl@1.0.so vendor/lib64/com.journeyOS.J007engine.hidl@1.0.so
adb shell killall com.journeyOS.J007engine.hidl@1.0-service
```