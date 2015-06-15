#!/bin/bash

clear
res1=$(date +%s.%N)
echo "==================="
echo "Drupal build script"
echo "-- Created by DROPTICA - www.droptica.com"
echo "-- Script version: 2013-09-08.01"
echo "Usage:"
echo "./build.sh without parameters assumes you have everything set up in conf/local"
echo "./build.sh PARAMETERS param1 param2 param3 - lets you enter parameters manually"
echo "==================="
echo "Starting ..."

RESULT=0

echo "Loading config..."

if [ -e "./conf/local/conf.inc" ]; then
  echo "Loading config file: "$(dirname $0)"/../conf/local/conf.inc"
  source $(dirname $0)"/../conf/local/conf.inc"
else
  echo "Missing file ./conf/local/conf.inc"
  echo "Error, exiting."
  exit;
fi
RESULT=$(($RESULT+$?))
if [ $RESULT = 1 ]; then
  exit 1
fi

PARAM_PROJECT_PATH_APP=$PARAM_PROJECT_PATH"/"$PARAM_DRUPAL_LOCATION
echo "Parameters: "
echo "- PARAM_PROJECT_PATH: ${PARAM_PROJECT_PATH}"
echo "- PARAM_PROJECT_PATH_APP: ${PARAM_PROJECT_PATH_APP}"
echo "- PARAM_SITE_URI: ${PARAM_SITE_URI}"
echo "- PARAM_SITE_DIRECTORY (sites/PARAM_SITE_DIRECTORY/settings.php): ${PARAM_SITE_DIRECTORY}"

  echo "drush dump"
  drush -r $PARAM_PROJECT_PATH_APP --uri=$PARAM_SITE_URI sql-dump --result-file=$PARAM_PROJECT_PATH"/databases/database.sql" -y
  
  echo "tar"
  cd $PARAM_PROJECT_PATH"/databases"
  tar -czvf database.sql.tar.gz database.sql
  rm database.sql
