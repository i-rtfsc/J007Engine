# Android Studio
全编后在Android根目录执行：
```bash 
# 把framework.jar导入到system_libs
make J007ServiceFrameworkStudio

# 把J007engine.jar导入到system_libs
make J007ServiceHidlStudio
```
可查看Android.mk如何实现该功能。

执行keystore.sh，生成系统签名文件。