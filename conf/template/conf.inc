#!/bin/sh
#Build parameters
# Version 2014-05-23.01

PARAM_DRUPAL_LOCATION="app"

PARAM_SITE_DIRECTORY=default

PARAM_SITE_URI=default

PARAM_PROJECT_IS_NEW="yes"

PARAM_INSTALLATION_PROFILE="myprofile"

PARAM_SITE_NAME="Site Name"

PARAM_ADMIN_NAME="admin"

PARAM_ADMIN_PASS="123"

PARAM_DBDRIVER="mysql"

PARAM_DBHOST="localhost"

PARAM_DBPORT=3306

# For existing project:

PARAM_DBUSER="root"
PARAM_DBPASS="root"
PARAM_DBNAME_DRUPAL="dbname"
PARAM_DBDUMP_FILE="database.sql.tar.gz"

# Dump: ask, yes or no
PARAM_DBDUMP_STATUS="ask"

# DB clear
PARAM_CLEAR_DATABASE="yes"

# stage file proxy
PARAM_ENABLE_STAGE_FILE_PROXY="no"
PARAM_STAGE_FILE_PROXY_URL="http://www.droptica.com"

# deleter txt files
PARAM_DELETE_TXT_FILES="yes"

# For drush make:

PARAM_MAKE_FILE="file.make"
PARAM_CUSTOM_MODULES_DIR="modules"
PARAM_CUSTOM_THEMES_DIR="themes"
PARAM_CUSTOM_INSTLATION_PROFILE="myprofile"

