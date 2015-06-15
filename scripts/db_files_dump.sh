#!/bin/bash

#TODO
# Dodać parametry do skryptu. To co tu jest to skopiowane komendy z ecollege z jenkinsa.

clear
res1=$(date +%s.%N)
echo "==================="
echo "Drupal db files dump script"
echo "-- Created by DROPTICA - www.droptica.com"
echo "-- Script version: 2014-05-28.01"
echo "Usage:"
echo "TODO"
echo "==================="
echo "Starting ..."

touch /home/projects_db_and_files/ecollege/database.tar.gz
touch /home/projects_db_and_files/ecollege/database.sql

cp /home/projects_db_and_files/ecollege/database.tar.gz /home/projects_db_and_files/ecollege/db-copy-$(date +%Y-%m-%d-%H.%M.%S).tar.gz
rm /home/projects_db_and_files/ecollege/database.tar.gz
rm /home/projects_db_and_files/ecollege/database.sql
mysqldump ecollege_stg > /home/projects_db_and_files/ecollege/database.sql -u ecollege_stg --password=ecollege_stg
tar zcfv /home/projects_db_and_files/ecollege/database.tar.gz /home/projects_db_and_files/ecollege/database.sql


touch /home/projects_db_and_files/ecollege/files.tar.gz
cp /home/projects_db_and_files/ecollege/files.tar.gz /home/projects_db_and_files/ecollege/files-copy-$(date +%Y-%m-%d-%H.%M.%S).tar.gz
rm /home/projects_db_and_files/ecollege/files.tar.gz

cd /var/lib/jenkins/workspace/ecollege_stg/app/sites/default/
tar zcfv /home/projects_db_and_files/ecollege/files.tar.gz files/
