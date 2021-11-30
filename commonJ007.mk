# Add J007Engine hidl & J007Service
PRODUCT_PACKAGES += \
    com.journeyOS.J007engine.hidl@1.0-service \
    J007Service

# Add J007EngineTest hidl & J007ServiceTest for debug building
PRODUCT_PACKAGES_DEBUG += \
    com.journeyOS.J007engine.hidl.test \
    J007ServiceTest
