chmod 777 keytool-importkeypair

KEY_PASSWORD=android
KEY_ALIAS=platform
KEYSTORE_PATH=../platform.jks

sh keytool-importkeypair -k $KEYSTORE_PATH -p $KEY_PASSWORD -pk8 $ANDROID_BUILD_TOP/build/target/product/security/platform.pk8 -cert $ANDROID_BUILD_TOP/build/target/product/security/platform.x509.pem -alias $KEY_ALIAS

cat > ../keystore.properties <<EOF
keyAlias=$KEY_ALIAS
keyPassword=$KEY_PASSWORD
storeFile=$KEYSTORE_PATH
storePassword=$KEY_ALIAS
EOF
