# Add J007Engine hidl to framework compatibility matrix
DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE += \
    vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/default/vendor_framework_compatibility_matrix.xml

# Add J007Engine hidl & J007Service sepolicy
-include vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/default/sepolicy/sepolicy.mk
-include vendor/journeyOS/proprietary/J007Engine/frameworks/sepolicy/sepolicy.mk