year := $(shell date +%Y)
month := $(shell date +%m)
day := $(shell date +%d)
build_date := $(shell date +%Y%m%d)
date_string=${year}/${month}/1
git_commit_id := $(shell git --git-dir $(LOCAL_PATH)/../../.git rev-parse --short HEAD)
#git_commit_count := $(shell git --git-dir $(LOCAL_PATH)/../../.git rev-list HEAD --count)
git_local_branch := $(shell git --git-dir $(LOCAL_PATH)/../../.git symbolic-ref --short -q HEAD)
git_commit_month_count := $(shell git --git-dir $(LOCAL_PATH)/../../.git rev-list --count --since=${date_string} --all)
git_first_commit_time := $(shell git --git-dir $(LOCAL_PATH)/../../.git log --reverse --format="format:%ci" | sed -n 1p)
git_first_commit_year=$(shell echo $(git_first_commit_time)|awk -F '-' '{ print $$1 }')

# Build the version code
# year * 100000 + month * 1000 + commitCount
# eg:202112001
build_version_code := $(shell expr ${year}00000 + ${month}000 + ${git_commit_month_count})

# Build the version name
#eg:v0.12.1.20211202.main.2b1af96
# build year reduce first commit year
version_name_major := $(shell expr ${year} - ${git_first_commit_year})
# build month
base_version_minor := $(month)
build_version_name := v$(version_name_major).$(base_version_minor).$(git_commit_month_count).$(build_date).$(git_local_branch).$(git_commit_id)

$(info "-------------------------------------------------------------------")
$(info $(build_version_code))
$(info $(build_version_name))
$(info "-------------------------------------------------------------------")
