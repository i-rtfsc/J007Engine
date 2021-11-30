/*
 * Copyright (c) 2021 anqi.huang@outlook.com
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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <unistd.h>
#include <math.h>

#define DEFAULT_PATH_SIZE 60
#define UNSUPPORTED (-1)

class Utils {
public:
    static int read_file_int(const char *from, int *to);

    static int read_file_str(const char *from, char *to);

    static int read_process_int(const char *cmd, int *result);

    static int read_process_str(const char *cmd, char *result);
};


#endif //_FILEHELPER_H
