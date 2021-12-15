#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# -*- encoding: utf-8 -*-
#
# Copyright (c) 2020 anqi.huang@outlook.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


import subprocess
import time
from datetime import datetime


class Git:
    git_dir = ''
    cmd_git_commit_id = "git --git-dir {dir}/.git rev-parse --short HEAD"
    cmd_git_local_branch = "git --git-dir {dir}/.git symbolic-ref --short -q HEAD"
    cmd_git_commit_month_count = "git --git-dir {dir}/.git rev-list --count --since=${time} --all"
    cmd_git_first_commit_time = "git --git-dir {dir}/.git log --reverse --format=\"format:%ci\" | sed -n 1p"

    year = ''
    month = ''
    day = ''

    def __init__(self, git_dir):
        self.git_dir = git_dir
        self.build_date = time.strftime("%Y%m%d", time.localtime())
        self.year = time.strftime("%Y", time.localtime())
        self.month = time.strftime("%m", time.localtime())
        self.day = time.strftime("%d", time.localtime())
        # print("year = %s , month = %s, day = %s " % (self.year, self.month, self.day))

    def running(self, cmd):
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        result = p.communicate()[0].decode("utf-8")
        return result.rstrip()

    def get_info(self):
        git_commit_id = self.running(self.cmd_git_commit_id.format(dir=self.git_dir))
        git_local_branch = self.running(self.cmd_git_local_branch.format(dir=self.git_dir))
        git_commit_month_count = self.running(
            self.cmd_git_commit_month_count.format(dir=self.git_dir, time=self.year + "/" + self.month + "/1"))

        git_first_commit_time = self.running(self.cmd_git_first_commit_time.format(dir=self.git_dir))
        git_first_commit_year = git_first_commit_time.split('-')[0]
        # print(
        #     "git_commit_id = %s , git_local_branch = %s, git_commit_month_count = %s "
        #     ", git_first_commit_time = %s , git_first_commit_year = %s "
        #     % (git_commit_id, git_local_branch, git_commit_month_count,
        #        git_first_commit_time, git_first_commit_year))

        build_version_code = int(self.year) * 10000 + int(self.month) * 100 + int(git_commit_month_count)
        version_name_major = int(self.year) - int(git_first_commit_year)
        build_version_name = 'v.' + str(
            version_name_major) + "." + self.month + "." + git_commit_month_count + "." + self.build_date + "." + git_local_branch + "." + git_commit_id
        print("build_version_code = %s , build_version_name = %s " % (build_version_code, build_version_name))
        return build_version_code, build_version_name

def main():
    git = Git('./vendor/journeyOS/proprietary/J007Engine')
    git.get_info()


if __name__ == "__main__":
    main()
