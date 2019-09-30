#!/bin/bash -e

if [ -d railwayman-firmware ]; then
  echo Removing previous packaging directory
  rm -rf railwayman-firmware/
fi

echo -------------
echo Packaging....
echo -------------
mkdir railwayman-firmware
mv src/controller/controller.hex railwayman-firmware
mv src/lighttest/lighttest.hex railwayman-firmware
mv src/section_driver/section_driver.hex railwayman-firmware

echo -------------
echo Uploading....
echo -------------
declare -x OUTPUT_FILE=railwayman-firmware-${CIRCLE_BUILD_NUM}.tar.gz
tar cv railwayman-firmware | gzip > ${OUTPUT_FILE}
sha256sum railwayman-firmware-${CIRCLE_BUILD_NUM}.tar.gz > ${OUTPUT_FILE}.sha
aws s3 cp ${OUTPUT_FILE} ${S3_UPLOAD_URI}/${CIRCLE_BUILD_NUM}/${OUTPUT_FILE} --acl public-read
aws s3 cp ${OUTPUT_FILE}.sha ${S3_UPLOAD_URI}/${CIRCLE_BUILD_NUM}/${OUTPUT_FILE}.sha --acl public-read
if [ "$CIRCLE_BRANCH" == "master" ]; then
  aws s3 cp ${OUTPUT_FILE} ${S3_UPLOAD_URI}/latest/${OUTPUT_FILE} --acl public-read
  aws s3 cp ${OUTPUT_FILE}.sha ${S3_UPLOAD_URI}/latest/${OUTPUT_FILE}.sha --acl public-read
fi
