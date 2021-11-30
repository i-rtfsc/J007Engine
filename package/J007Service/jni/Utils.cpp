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

#include "Utils.h"

int Utils::read_file_int(const char *path, int *result) {
    FILE *freqfile = fopen(path, "r");
    if (freqfile == NULL)
        return UNSUPPORTED;
    fscanf(freqfile, "%d", result);
    fclose(freqfile);
    return 0;
}

int Utils::read_file_str(const char *path, char *result) {
    FILE *freqfile = fopen(path, "r");
    if (freqfile == NULL)
        return UNSUPPORTED;
    fscanf(freqfile, "%s", result);
    fclose(freqfile);
    return 0;
}

int Utils::read_process_int(const char *cmd, int *result) {
    FILE *process = popen(cmd, "r");
    if (process == NULL)
        return UNSUPPORTED;
    fscanf(process, "%d", result);
    pclose(process);
    return 0;
}

int Utils::read_process_str(const char *cmd, char *result) {
    FILE *process = popen(cmd, "r");
    if (process == NULL)
        return UNSUPPORTED;
    fscanf(process, "%s", result);
    pclose(process);
    return 0;
}

