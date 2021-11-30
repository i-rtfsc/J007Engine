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

#include "log.h"
#include "utils.h"

#undef  LOG_TAG
#define LOG_TAG        "J007Engine-Utils"

char *Utils::read_file(const char *f_name, int *err, size_t *f_size) {
    char *buffer;
    size_t length;
    FILE *f = fopen(f_name, "rb");
    size_t read_length;

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        // 1 GiB; best not to load a hole large file in one string
        if (length > 1073741824) {
            *err = FILE_TO_LARGE;

            return NULL;
        }

        buffer = (char *) malloc(length + 1);

        if (length) {
            read_length = fread(buffer, 1, length, f);

            if (length != read_length) {
                *err = FILE_READ_ERROR;

                return NULL;
            }
        }

        fclose(f);

        *err = FILE_OK;
        buffer[length] = '\0';
        *f_size = length;
    } else {
        *err = FILE_NOT_EXIST;

        return NULL;
    }

    return buffer;
}

string Utils::readFile(string file) {
    int fd;
    char buffer[1024];
    string str = "";

    if (file.empty())
        return str;

    if ((fd = TEMP_FAILURE_RETRY(open(file.c_str(), O_RDONLY))) <= 0) {
        LOGE("open file %s failed.", file.c_str());
        return str;
    }

    memset(buffer, 0, sizeof(buffer));
    size_t size = (size_t) TEMP_FAILURE_RETRY(read(fd, buffer, sizeof(buffer) - 1));
    // for(size_t i = 0; i < size; i++) {
    //     if((buffer[i] == '\r') || (buffer[i] == '\n') || (buffer[i] == '\t')) {
    //         buffer[i] = 0;
    //         break;
    //     }
    // }
    str = string(buffer);
    ALOGI("read %s = %s ", file.c_str(), str.c_str());
    return str;
}

bool Utils::writeFile(string file, string content) {
    int fd;
    size_t size, left;

    if (file.empty())
        return 0;

    if ((fd = TEMP_FAILURE_RETRY(open(file.c_str(), O_RDWR))) <= 0) {
        LOGE("open file %s failed.", file.c_str());
        return 0;
    }

    const char *p = content.data();
    size = left = content.size();
    while (left > 0) {
        ssize_t n = TEMP_FAILURE_RETRY(write(fd, p, left));
        if (n == -1)
            break;

        p += n;
        left -= n;
    }

    int ret = 0;
    if ((size - left) != 1) {
        ret = -1;
    }

    return ret == 0;
}

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

vector <string> Utils::get_list_of_files(string folderName, bool bAbsolutePath) {
    ALOGD("get list of files = %s", folderName.c_str());
    vector <string> listOfFiles;

    struct dirent *directoryEntry;
    DIR *directory = opendir(folderName.c_str());
    if (directory != NULL) {
        while (true) {
            directoryEntry = readdir(directory);

            if (directoryEntry == NULL) break;
            string entry = string(directoryEntry->d_name);

            if ((entry.compare(".") == 0) || (entry.compare("..") == 0)) {
                continue;
            }

            if (bAbsolutePath) {
                entry = folderName + "/" + entry;
            }
            listOfFiles.push_back(entry);
        }
        closedir(directory);
    }

    return listOfFiles;
}