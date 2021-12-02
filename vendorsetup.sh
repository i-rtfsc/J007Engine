#!/bin/bash

# josbuild script version:0.1

declare JOS_BUILD_LAST_CHOICE_1
declare JOS_BUILD_LAST_CHOICE_2
declare JOS_BUILD_LAST_FUNC_RETURN
declare JOS_BUILD_LOG_DIR
declare JOS_BUILD_LOG_SUFFIX
declare JOS_BUILD_BUILD_THREADS
declare JOS_BUILD_LOG_SUFFIX_FOR_USER

function josbuild() {
    if [ -z $TARGET_PRODUCT ]; then
      echo "lunch target product first!"
      return
    fi

    export JOS_BUILD_BUILD_THREADS=
    export JOS_BUILD_LOG_SUFFIX=
    export JOS_BUILD_LOG_SUFFIX_FOR_USER=
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
    # init log default for push install file to device
    _josbuild_init_log_for_install

    echo
    echo ================================
    echo josbuild version:0.5
    echo build script for josbuild
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        echo -j $JOS_BUILD_BUILD_THREADS
    fi
    if [ -n "$JOS_BUILD_LOG_SUFFIX_FOR_USER" ]; then
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
    export JOS_BUILD_LOG_SUFFIX_FOR_USER=`date +%H%S%S`
}

function _josbuild_init_log_for_install() {
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

function _josbuild_allbuild() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j$JOS_BUILD_BUILD_THREADS"
    fi

    echo "m $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log"
    m $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log
}

function _josbuild_ninjia() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j$JOS_BUILD_BUILD_THREADS"
    fi

    echo "time prebuilts/build-tools/linux-x86/bin/ninja $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log"
    time prebuilts/build-tools/linux-x86/bin/ninja $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log
}

function _josbuild_make() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j$JOS_BUILD_BUILD_THREADS"
    fi

    echo "make $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log"
    make $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log
}

function _josbuild_mmm() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j$JOS_BUILD_BUILD_THREADS"
    fi

    echo "mmm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log"
    mmm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log
}

function _josbuild_mm() {
    local param_1=;
    if [ -n "$JOS_BUILD_BUILD_THREADS" ]; then
        param_1="-j$JOS_BUILD_BUILD_THREADS"
    fi

    echo "mm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log"
    mm $* $param_1 2>&1 | tee $JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log
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
    do_push_file
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
    do_push_file
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
    do_push_file
}

function josbuild_003_mm_build()
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
    _josbuild_003_mm_build_$selection
    do_push_file
}

function _josbuild_003_mm_build_J007Engine()
{
    _josbuild_mm vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/default/
}

function _josbuild_003_mm_build_J007Service()
{
    _josbuild_mm vendor/journeyOS/proprietary/J007Engine/package/J007Service/
}

function _josbuild_003_mm_build_J007ServiceTest()
{
    _josbuild_mm vendor/journeyOS/proprietary/J007Engine/package/J007ServiceTest/
}

function _josbuild_003_mm_build_J007engineInterface()
{
    _josbuild_mm vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/
}

function josbuild_004_mmm_build()
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
    _josbuild_004_mmm_build_$selection
    do_push_file
}

function _josbuild_004_mmm_build_J007Engine()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/default/
}

function _josbuild_004_mmm_build_J007Service()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/package/J007Service/
}

function _josbuild_004_mmm_build_J007ServiceTest()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/package/J007ServiceTest/
}

function _josbuild_004_mmm_build_J007engineInterface()
{
    _josbuild_mmm vendor/journeyOS/proprietary/J007Engine/interfaces/hidl/1.0/
}

function josbuild_005_make_android_studio()
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

function _josbuild_005_make_android_studio_J007ServiceFrameworkStudio()
{
    _josbuild_make J007ServiceFrameworkStudio
}

function _josbuild_005_make_android_studio_J007ServiceHidlStudio()
{
    _josbuild_make J007ServiceHidlStudio
}

function _josbuild_005_make_android_studio_J007ServiceFrameworkStudioTest()
{
    _josbuild_make J007ServiceFrameworkStudioTest
}

# Credit for color strip sed: http://goo.gl/BoIcm
function do_push_file()
{
    adb start-server # Prevent unexpected starting server message from adb get-state in the next line
    if ! _adb_connected; then
        echo "No device is online. Waiting for one..."
        echo "Please connect USB and/or enable USB debugging"
        until _adb_connected; do
            sleep 1
        done
        echo "Device Found."
    fi

    # retrieve IP and PORT info if we're using a TCP connection
    TCPIPPORT=$(adb devices \
        | egrep '^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9]):[0-9]+[^0-9]+' \
        | head -1 | awk '{print $1}')
    echo $TCPIPPORT
    adb root &> /dev/null
    sleep 0.3
    if [ -n "$TCPIPPORT" ]
    then
        # adb root just killed our connection
        # so reconnect...
        adb connect "$TCPIPPORT"
    fi
    adb wait-for-device &> /dev/null
    adb remount &> /dev/null

    _local_log_file=$JOS_BUILD_LOG_DIR${JOS_BUILD_LAST_CHOICE_2}_$JOS_BUILD_LOG_SUFFIX.log
    #echo $_local_log_file
    is_gnu_sed=`sed --version | head -1 | grep -c GNU`

    # Install: <file>
    if [ $is_gnu_sed -gt 0 ]; then
        LOC="$(cat $_local_log_file | sed -r -e 's/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]//g' -e 's/^\[ {0,2}[0-9]{1,3}% [0-9]{1,6}\/[0-9]{1,6}\] +//' \
            | grep '^Install: ' | cut -d ':' -f 2)"
    else
        LOC="$(cat $_local_log_file | sed -E "s/"$'\E'"\[([0-9]{1,3}((;[0-9]{1,3})*)?)?[m|K]//g" -E "s/^\[ {0,2}[0-9]{1,3}% [0-9]{1,6}\/[0-9]{1,6}\] +//" \
            | grep '^Install: ' | cut -d ':' -f 2)"
    fi

    # Copy: <file>
    if [ $is_gnu_sed -gt 0 ]; then
        LOC="$LOC $(cat $_local_log_file | sed -r -e 's/\x1B\[([0-9]{1,2}(;[0-9]{1,2})?)?[m|K]//g' -e 's/^\[ {0,2}[0-9]{1,3}% [0-9]{1,6}\/[0-9]{1,6}\] +//' \
            | grep '^Copy: ' | cut -d ':' -f 2)"
    else
        LOC="$LOC $(cat $_local_log_file | sed -E "s/"$'\E'"\[([0-9]{1,3}((;[0-9]{1,3})*)?)?[m|K]//g" -E 's/^\[ {0,2}[0-9]{1,3}% [0-9]{1,6}\/[0-9]{1,6}\] +//' \
            | grep '^Copy: ' | cut -d ':' -f 2)"
    fi

    ###########################for ninja build###########################
    if [ -n "$LOC" ]; then
        LOC="$LOC $(cat $_local_log_file | grep 'Install: ' | cut -d ':' -f 2)"
    fi

    if [ -n "$LOC" ]; then
        LOC="$LOC $(cat $_local_log_file | grep 'Copy: ' | cut -d ':' -f 2)"
    fi
    ###########################for ninja build###########################

    # If any files are going to /data, push an octal file permissions reader to device
    if [ -n "$(echo $LOC | egrep '(^|\s)/data')" ]; then
        CHKPERM="/data/local/tmp/chkfileperm.sh"
(
cat <<'EOF'
#!/system/xbin/sh
FILE=$@
if [ -e $FILE ]; then
    ls -l $FILE | awk '{k=0;for(i=0;i<=8;i++)k+=((substr($1,i+2,1)~/[rwx]/)*2^(8-i));if(k)printf("%0o ",k);print}' | cut -d ' ' -f1
fi
EOF
) > $OUT/.chkfileperm.sh
        echo "Pushing file permissions checker to device"
        adb push $OUT/.chkfileperm.sh $CHKPERM
        adb shell chmod 755 $CHKPERM
        rm -f $OUT/.chkfileperm.sh
    fi

    RELOUT=$(echo $OUT | sed "s#^${ANDROID_BUILD_TOP}/##")

    stop_n_start=false
    for TARGET in $(echo $LOC | tr " " "\n" | sed "s#.*${RELOUT}##" | sort | uniq); do
        # Make sure file is in $OUT/{system,system_ext,data,odm,oem,product,product_services,vendor}
        case $TARGET in
            /system/*|/system_ext/*|/data/*|/odm/*|/oem/*|/product/*|/product_services/*|/vendor/*)
                # Get out file from target (i.e. /system/bin/adb)
                FILE=$OUT$TARGET
            ;;
            *) continue ;;
        esac

        case $TARGET in
            /data/*)
                # fs_config only sets permissions and se labels for files pushed to /system
                if [ -n "$CHKPERM" ]; then
                    OLDPERM=$(adb shell $CHKPERM $TARGET)
                    OLDPERM=$(echo $OLDPERM | tr -d '\r' | tr -d '\n')
                    OLDOWN=$(adb shell ls -al $TARGET | awk '{print $2}')
                    OLDGRP=$(adb shell ls -al $TARGET | awk '{print $3}')
                fi
                echo "Pushing: $FILE $TARGET"
                adb push $FILE $TARGET
                if [ -n "$OLDPERM" ]; then
                    echo "Setting file permissions: $OLDPERM, $OLDOWN":"$OLDGRP"
                    adb shell chown "$OLDOWN":"$OLDGRP" $TARGET
                    adb shell chmod "$OLDPERM" $TARGET
                else
                    echo "$TARGET did not exist previously, you should set file permissions manually"
                fi
                adb shell restorecon "$TARGET"
            ;;
            /system/priv-app/SystemUI/SystemUI.apk|/system/framework/*)
                # Only need to stop services once
                if ! $stop_n_start; then
                    adb shell stop
                    stop_n_start=true
                fi
                echo "Pushing: $FILE $TARGET"
                adb push $FILE $TARGET
            ;;
            *)
                echo "Pushing: $FILE $TARGET"
                adb push $FILE $TARGET
            ;;
        esac
    done
    if [ -n "$CHKPERM" ]; then
        adb shell rm $CHKPERM
    fi
    if $stop_n_start; then
        adb shell start
    fi
    #delete file if need
    if [ -n "$JOS_BUILD_LOG_SUFFIX_FOR_USER" ]; then
        #echo "usr need log file, don't need delete"
        echo ""
    else
        rm -f $_local_log_file
    fi
}