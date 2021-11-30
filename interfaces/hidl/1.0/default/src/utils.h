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

#include <unistd.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>

#include <iostream>
#include <vector>
#include <fstream>

#define UNUSED __attribute__((__unused__))

#define FILE_OK 0
#define FILE_NOT_EXIST 1
#define FILE_TO_LARGE 2
#define FILE_READ_ERROR 3

#define DEFAULT_PATH_SIZE 60
#define UNSUPPORTED (-1)

using namespace std;

class Utils {
public:
    static string readFile(string file);

    static bool writeFile(string file, string content);

    static int read_file_int(const char *from, int *to);

    static int read_file_str(const char *from, char *to);

    static int read_process_int(const char *cmd, int *result);

    static int read_process_str(const char *cmd, char *result);

    static vector <string> get_list_of_files(string folderName, bool bAbsolutePath);

private:
    static char *read_file(const char *f_name, int *err, size_t *f_size);
};

#endif