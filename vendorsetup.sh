#!/bin/bash

# josbuild script version:0.1

declare JOS_BUILD_LAST_CHOICE_1
declare JOS_BUILD_LAST_CHOICE_2
declare JOS_BUILD_LAST_FUNC_RETURN
declare JOS_BUILD_LOG_DIR
declare JOS_BUILD_LOG_SUFFIX
declare JOS_BUILD_BUILD_THREADS

function josbuild() {
    if [ -z $TARGET_PRODUCT ]; then
      echo "lunch target product first!"
      return
    fi

#    echo "---------------------------------test---------------------------------"
#    echo $TARGET_PRODUCT
#    echo $ANDROID_BUILD_TOP
#    echo "---------------------------------test---------------------------------"

    export JOS_BUILD_BUILD_THREADS=
    export JOS_BUILD_LOG_SUFFIX=
    local target
    while [ $# -gt 0 ]; do
        case $1 in
            -h|--help)
                echo "josbuild [-l|-j] [target]"
                return
                ;;
            -l|--log)
                _josbuild_init_log
                shift 1
                ;;
            -j)
                export JOS_BUILD_BUILD_THREADS=$2
                shift 2
                ;;
            *)
                target=$1
                shift
                ;;
        esac
    done

    echo
    echo ================================
    echo josbuild version:0.2
    echo build script for josbuild
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        echo -j $JOS_BUILD_BUILD_THREADS
    fi
    if [ -n "$JOS_BUILD_LOG_SUFFIX" ]; then
        echo -l
    fi
    if [ -n "$target" ]; then
        echo $target
    fi
    echo ================================
    echo
    echo

    local choices=`typeset -F | grep "declare -f josbuild_..._" | awk '{print $3}'`
    local index_start=0
    local choice_cut=13
    local answer=
    local selection=

    _osbuild_clean_env

    if [ "$target" ] ; then
        answer=$1
    else
        echo "(lastbuild) $JOS_BUILD_LAST_CHOICE_1 $JOS_BUILD_LAST_CHOICE_2" && echo
        _josbuild_show_and_choose_combo "$choices" $choice_cut $index_start "josbuild menu(select options):"
        answer=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    _josbuild_parse_answer "$choices" $choice_cut $index_start $answer
    selection=$JOS_BUILD_LAST_FUNC_RETURN

    if [ ${selection:$choice_cut} != "lastbuild" ]; then
        export JOS_BUILD_LAST_CHOICE_1=${selection:13}
        export JOS_BUILD_LAST_CHOICE_2=
    fi

    # call function according to the selection
    $selection
}

function _josbuild_show_and_choose_combo() {
    local choices=$1
    local choice_cut=$2
    local index=$3
    local title=$4
    local answer=
    local def_answer=
    local c=

    # print combo menu, all items are picked from functions which started with josbuild_digit_ in this script
    echo "$title"
    for c in $choices; do
        if [ -z $def_answer ]; then
            def_answer=$c
        fi
        printf "%4s. %s\n" $index ${c:$choice_cut}
        index=$(($index+1))
    done
    printf "Which would you like? [ %s ] " ${def_answer:$choice_cut}
    read answer

    if [ -z "$answer" ]; then
        answer=${def_answer:$choice_cut}
    fi
    export JOS_BUILD_LAST_FUNC_RETURN=$answer
}

function _josbuild_parse_answer() {
    local choices=$1
    local choice_cut=$2
    local index=$3
    local answer=$4
    local c=

    if [ -z "$answer" ]
    then
        echo "error: _josbuild_parse_answer() get null answer."
    elif (echo -n $answer | grep -q -e "^[0-9][0-9]*$")
    then
        for c in $choices; do
            if [ $answer == $index ]; then
                selection=$c
                break
            fi
            index=$(($index+1))
        done
    else
        for c in $choices; do
            if [ "$answer" == "${c:$choice_cut}" ]; then
                selection=$c
                break
            fi
        done
    fi
    printf "\n    SELECTED: %s\n\n" ${selection:$choice_cut}

    export JOS_BUILD_LAST_FUNC_RETURN=$selection
}

function _josbuild_init_log() {
    #script_dir="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
    export JOS_BUILD_LOG_DIR=$(pwd)"/out/josbuild_"
    export JOS_BUILD_LOG_SUFFIX=`date +%H%S%S`
    # bash script mkdir permission denied
    # /out/osbuild/ ---> /out/osbuild_
    # mkdir -p $JOS_BUILD_LOG_DIR
}

function _josbuild_clean_env() {
    # env
    unset XXX
}

function _josbuild_envsetup_lunch() {
    # source and lunch
    source build/envsetup.sh
    lunch lineage_$1
}

function _josbuild_allbuild() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j $JOS_BUILD_BUILD_THREADS"
    fi

    if [ -n "$JOS_BUILD_LOG_SUFFIX" ]; then
        echo "$param_1 2>&1 | tee $JOS_BUILD_LOG_DIR$JOS_BUILD_LOG_SUFFIX.log"
        make $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR$JOS_BUILD_LOG_SUFFIX.log
    else
        echo $param_1
        make $* $param_1
    fi
}

function _josbuild_ninjia() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j $JOS_BUILD_BUILD_THREADS"
    fi

    if [ -n "$JOS_BUILD_LOG_SUFFIX" ]; then
        echo "time prebuilts/build-tools/linux-x86/bin/ninja $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log"
        time prebuilts/build-tools/linux-x86/bin/ninja $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log
    else
        echo time prebuilts/build-tools/linux-x86/bin/ninja $* $param_1
        time prebuilts/build-tools/linux-x86/bin/ninja $* $param_1
    fi
}

function _josbuild_make() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j $JOS_BUILD_BUILD_THREADS"
    fi

    if [ -n "$JOS_BUILD_LOG_SUFFIX" ]; then
        echo "make $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log"
        make $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log
    else
        echo make $param_1
        make $param_1
    fi
}

function _josbuild_mmm() {
    local param_1=$1;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j $JOS_BUILD_BUILD_THREADS"
    fi

    if [ -n "$JOS_BUILD_LOG_SUFFIX" ]; then

        echo "mmm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log"
        mmm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log
    else
        echo mmm $param_1
        mmm $param_1
    fi
}

function _josbuild_mm() {
    local param_1=$1;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j $JOS_BUILD_BUILD_THREADS"
    fi

    if [ -n "$JOS_BUILD_LOG_SUFFIX" ]; then
        echo "mm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log"
        mm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${selection}_$JOS_BUILD_LOG_SUFFIX.log
    else
        echo $param_1
        mm $param_1
    fi
}

function josbuild_000_lastbuild()
{
    if [ -z $JOS_BUILD_LAST_CHOICE_1 ]; then
        echo "Has not built any target yet. please choose one first!"
        return
    fi
    local choices=`typeset -F | grep "declare -f josbuild_..._" | awk '{print $3}'`
    for c in $choices; do
        if [ "$JOS_BUILD_LAST_CHOICE_1" == "${c:13}" ]; then
            selection=$c
            break
        fi
    done
    echo "**************** lastbuild ******************"
    echo "$JOS_BUILD_LAST_CHOICE_1 $JOS_BUILD_LAST_CHOICE_2"
    echo "*********************************************"

    # call function according to the selection
    $selection "$JOS_BUILD_LAST_CHOICE_2"
}

function josbuild_001_ninja_build()
{
    local choices="com.journeyOS.J007engine.hidl@1.0-service J007Service J007ServiceTest"
    local index_start=1
    local choice_cut=0
    local answer=
    local selection=

    if [ "$1" ] ; then
        answer=$1
    else
        _josbuild_show_and_choose_combo "$choices" $choice_cut $index_start "module(s) menu(select options):"
        answer=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    if (echo -n $answer | grep -q -e ". .")
    then
       selection=$answer
    else
        _josbuild_parse_answer "$choices $answer" $choice_cut $index_start $answer
        selection=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    if [ $JOS_BUILD_LAST_CHOICE_1 != "lastbuild" ]; then
        export JOS_BUILD_LAST_CHOICE_2=${selection}
    fi

    _josbuild_ninjia -f out/combined-${TARGET_PRODUCT}.ninja ${selection}
    #_josbuild_ninjia -f out/combined-lineage_lemonadep.ninja ${selection}
}

function josbuild_002_make_build()
{
    local choices="com.journeyOS.J007engine.hidl@1.0-service J007Service J007ServiceTest"
    local index_start=1
    local choice_cut=0
    local answer=
    local selection=

    if [ "$1" ] ; then
        answer=$1
    else
        _josbuild_show_and_choose_combo "$choices" $choice_cut $index_start "module(s) menu(select options):"
        answer=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    if (echo -n $answer | grep -q -e ". .")
    then
       selection=$answer
    else
        _josbuild_parse_answer "$choices $answer" $choice_cut $index_start $answer
        selection=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    if [ $JOS_BUILD_LAST_CHOICE_1 != "lastbuild" ]; then
        export JOS_BUILD_LAST_CHOICE_2=${selection}
    fi

    _josbuild_make ${selection}
}

function josbuild_003_m_build()
{
    local choices="J007Engine J007Service J007ServiceTest J007engineInterface"
    local index_start=1
    local choice_cut=0
    local answer=
    local selection=

    if [ "$1" ] ; then
        answer=$1
    else
        _josbuild_show_and_choose_combo "$choices" $choice_cut $index_start "module(s) menu(select options):"
        answer=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    _josbuild_parse_answer "$choices" $choice_cut $index_start $answer
    selection=$JOS_BUILD_LAST_FUNC_RETURN

    if [ $JOS_BUILD_LAST_CHOICE_1 != "lastbuild" ]; then
        export JOS_BUILD_LAST_CHOICE_2=${selection}
    fi

    echo $selection
    _josbuild_003_m_build_$selection
}

function _josbuild_003_m_build_007Engine()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/default/
}

function _josbuild_003_m_build_J007Service()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/package/J007Service/
}

function _josbuild_003_m_build_J007ServiceTest()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/package/J007ServiceTest/
}

function _josbuild_003_m_build_J007engineInterface()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/
}

function josbuild_004_make_android_studio()
{
    local choices="J007ServiceFrameworkStudio J007ServiceHidlStudio J007ServiceFrameworkStudioTest"
    local index_start=1
    local choice_cut=0
    local answer=
    local selection=

    if [ "$1" ] ; then
        answer=$1
    else
        _josbuild_show_and_choose_combo "$choices" $choice_cut $index_start "module(s) menu(select options):"
        answer=$JOS_BUILD_LAST_FUNC_RETURN
    fi

    _josbuild_parse_answer "$choices" $choice_cut $index_start $answer
    selection=$JOS_BUILD_LAST_FUNC_RETURN

    if [ $JOS_BUILD_LAST_CHOICE_1 != "lastbuild" ]; then
        export JOS_BUILD_LAST_CHOICE_2=${selection}
    fi

    echo $selection
    _josbuild_004_make_android_studio_$selection
}

function _josbuild_004_make_android_studio_J007ServiceFrameworkStudio()
{
    _josbuild_make J007ServiceFrameworkStudio
}

function _josbuild_004_make_android_studio_J007ServiceHidlStudio()
{
    _josbuild_make J007ServiceHidlStudio
}

function _josbuild_004_make_android_studio_J007ServiceFrameworkStudioTest()
{
    _josbuild_make J007ServiceFrameworkStudioTest
}
