# Add J007Engine config files
PRODUCT_COPY_FILES += \
    vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/default/config/cpuset.json:/vendor/etc/j007_engine/cpuset.json

-include vendor/journeyOS/proprietary/J007Engine/build_config.mk

# Add J007Engine hidl & J007Service
J007_ENGINE := com.journeyOS.J007engine.hidl@1.0-service
J007_ENGINE += J007Service
PRODUCT_PACKAGES += $(J007_ENGINE)

# Add J007EngineTest hidl & J007ServiceTest for debug building
PRODUCT_PACKAGES_DEBUG += \
    com.journeyOS.J007engine.hidl.test \
    J007ServiceTest
