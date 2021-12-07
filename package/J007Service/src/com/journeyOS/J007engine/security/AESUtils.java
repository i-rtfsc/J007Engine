/*
 * Copyright (c) 2018 anqi.huang@outlook.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.journeyOS.J007engine.security;

import android.util.Base64;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class AESUtils {
    private static String KEY = "AD42F6697B035B7580E4FEF93BE20BAD";
    private static String CHARSET = "utf-8";
    private static int OFFSET = 16;
    private static String TRANSFORMATION = "AES/CBC/PKCS5Padding";
    private static String ALGORITHM = "AES";

    public static String encrypt(String content) {
        return encrypt(content, KEY);
    }

    public static String decrypt(String content) {
        return decrypt(content, KEY);
    }

    private static String encrypt(String content, String key) {
        try {
            SecretKeySpec skey = new SecretKeySpec(key.getBytes(), ALGORITHM);
            IvParameterSpec iv = new IvParameterSpec(key.getBytes(), 0, OFFSET);

            Cipher cipher = Cipher.getInstance(TRANSFORMATION);
            byte[] byteContent = content.getBytes(CHARSET);
            cipher.init(Cipher.ENCRYPT_MODE, skey, iv);
            byte[] result = Base64.encode(cipher.doFinal(byteContent), Base64.DEFAULT);
            if (result != null) {
                return new String(result);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    private static String decrypt(String content, String key) {
        try {
            SecretKeySpec skey = new SecretKeySpec(key.getBytes(), ALGORITHM);
            IvParameterSpec iv = new IvParameterSpec(key.getBytes(), 0, OFFSET);

            Cipher cipher = Cipher.getInstance(TRANSFORMATION);
            cipher.init(Cipher.DECRYPT_MODE, skey, iv);
            byte[] result = cipher.doFinal(Base64.decode(content, Base64.DEFAULT));
            return new String(result);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
