MODEL="bx-framework"
cp $ANDROID_BUILD_TOP/out/target/common/obj/JAVA_LIBRARIES/${MODEL}_intermediates/classes.jar ../system_libs/${MODEL}.jar

MODEL_FRAMEWORK="framework-minus-apex"
cp $ANDROID_BUILD_TOP/out/target/common/obj/JAVA_LIBRARIES/${MODEL_FRAMEWORK}_intermediates/classes.jar ../system_libs/${MODEL_FRAMEWORK}.jar
