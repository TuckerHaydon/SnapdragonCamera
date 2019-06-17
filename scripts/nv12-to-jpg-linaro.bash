#!/bin/bash

# Author: Tucker Haydon
# Converts all images from a specified directory from nv12 format to jpg format
# using avconv. To be used on the snapdragon only

PROGNAME=$0
EXIT_FAILURE=1
EXIT_SUCCESS=0

usage() {
    echo "Usage: 
    $PROGNAME [-s <size; WxH>] [-i <input dir>] [-o <output dir>]

    Converts all images from nv12 format to jpg format. All images must end
    with the extension .yuv.  Must specify the source and destination
    directories. If the destination directory does not exit, it will be
    created." 1>&2;
    exit $EXIT_FAILURE
}

# Parse command line opts
while getopts ":s:i:o:" opt; do
  case $opt in
    (s) image_size=$OPTARG
        ;;
    (i) input_dir=$OPTARG
        ;;
    (o) output_dir=$OPTARG
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
   [[ -z ${input_dir+x} ]] || \
   [[ -z ${output_dir+x} ]]
then
    echo "All arguments requried." 1>&2
    usage
fi

# Check directories exist
if [[ ! -d "$input_dir" ]]; then 
    echo "Input directory does not exist."
    usage
fi

if [[ ! -d "$output_dir" ]]; then 
    echo "Output directory does not exist. Creating ${output_dir}..."
    mkdir -p ${output_dir}
fi

# Convert images
# Sequential foor loop
# for f in ${input_dir}/*.yuv; do 
#     name=$(basename ${f} .yuv)
#     avconv -v quiet -pix_fmt nv12 -s ${image_size} -i ${f} -qscale 1 "${output_dir}/${name}.jpg"
# done

# Parallelized xargs
# qscale 1 is max quality
# v quiet disables output
# format is nv12
ls ${input_dir}/*.yuv | xargs --replace={} --max-procs=4 \
  sh -c 'base=$(basename $1 .yuv); avconv -v quiet -pix_fmt nv12 -s ${2} -i ${1} -qscale 1 "${3}/${base}.jpg"' \
  -- {} ${image_size} ${output_dir}

exit $EXIT_SUCCESS

