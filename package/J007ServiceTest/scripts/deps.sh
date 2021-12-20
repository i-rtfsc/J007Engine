mkdir -p ../system_libs/

MODEL="jos-framework"
cp $ANDROID_BUILD_TOP/out/target/common/obj/JAVA_LIBRARIES/${MODEL}_intermediates/classes.jar ../system_libs/${MODEL}.jar
