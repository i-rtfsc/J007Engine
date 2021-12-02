# 简介
- interfaces：hidl服务、包括需要的se权限（还包含测试bin）
- package：java服务、测试app
- frameworks：java服务需要的se权限、sdk(可加到framework.jar)

这个方案是一个仓内完成99.99999%的代码，执行如下步骤就可编译到rom中。Android大版本升级时能最快适配。


# 编译到rom

```bash
-include vendor/journeyOS/proprietary/J007Engine/BoardConfigJ007.mk

$(call inherit-product, vendor/journeyOS/proprietary/J007Engine/commonJ007.mk)
或者
-include vendor/journeyOS/proprietary/J007Engine/commonJ007.mk
```

# java sdk
在famework/base/目录的Android.bp里引用：
```bash
srcs: [
   ...
   ":J007_engine_framework_aidl",
   ":J007_engine_framework_base_java",
   ...
],
```
