/*
 * Copyright (c) 2020 game dock developers
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

#ifndef _FILEHELPER_H
#define _FILEHELPER_H

class FileHelper {
private:
    const char *mFileName;

    int openFile(int flags);

    int closeFile(int fd);

public:
    FileHelper(const char *fileName);

    ~FileHelper();

    int readFile(char *buf, int size);

    int writeFile(const char *buf, int size);

};

#endif //_FILEHELPER_H
