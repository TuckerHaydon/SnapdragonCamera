#!/bin/bash

# Author: Tucker Haydon
# Program description

PROGNAME=$0
EXIT_FAILURE=1
EXIT_SUCCESS=0

usage() {
    echo "Usage: 
    $PROGNAME [-s <desired_image_size; WxH>] [-i <input_dir>]

    Crops images and removes the green bar present at the bottom of images."
    1>&2;
    exit $EXIT_FAILURE
}

# Parse command line opts
while getopts ":s:i:o:" opt; do
  case $opt in
    (s) image_size=$OPTARG
        ;;
    (i) input_dir=$OPTARG
        ;;
    (h) usage
        ;;
    \?) usage
        ;;
    :) usage
        ;;
    *) usage
        ;;
  esac
done
shift "$((OPTIND - 1))"

# Check no extra params
if [[ $# -ne 0 ]]; then
    usage
fi

# Check all args present
if [[ -z ${image_size+x} ]] || \
   [[ -z ${input_dir+x} ]]
then
    echo "All arguments requried." 1>&2
    usage
fi

# Crop images
# Sequential
# for f in ${input_dir}/*.jpg; do
#     convert $f -crop "${image_size}+0+0" $f
# done

# Parallelized xargs
ls ${input_dir}/*.jpg | xargs --replace={} --max-procs=4 convert {} -crop "${image_size}+0+0" {}

exit $EXIT_SUCCESS
