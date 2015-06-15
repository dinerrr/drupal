#!/bin/bash

clear
res1=$(date +%s.%N)
echo "==================="
echo "Drupal build script"
echo "-- Created by DROPTICA - www.droptica.com"
echo "-- Script version: 2014-06-24.01"
echo "Usage:"
echo "./build.sh without parameters assumes you have everything set up in conf/local"
echo "./build.sh PARAMETERS param1 param2 param3 - lets you enter parameters manually"
echo "==================="
echo "Starting ..."

RESULT=0

PARAM_PROJECT_PATH="$( cd "$(dirname "$0")"/../ ; pwd -P )"

echo "Loading config..."

if [ -e "$PARAM_PROJECT_PATH/conf/local/conf.inc" ]; then
  echo "Loading config file: $PARAM_PROJECT_PATH/conf/local/conf.inc"
  source "$PARAM_PROJECT_PATH/conf/local/conf.inc"
else
  echo "Missing file $PARAM_PROJECT_PATH/conf/local/conf.inc"
  echo "Error, exiting."
  exit 1
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

source "$(dirname $0)/git-hooks.inc.sh"

if [ $PARAM_PROJECT_IS_NEW = "make" ]; then
  chmod a+w -Rf $PARAM_PROJECT_PATH_APP
  rm -rf  $PARAM_PROJECT_PATH_APP
  drush -r $PARAM_PROJECT_PATH make $PARAM_MAKE_FILE $PARAM_PROJECT_PATH_APP
  chmod a+w $PARAM_PROJECT_PATH_APP/sites/default/files
  if [ ! -z $PARAM_CUSTOM_MODULES_DIR ]; then
    ln -s $PARAM_PROJECT_PATH/$PARAM_CUSTOM_MODULES_DIR $PARAM_PROJECT_PATH_APP/sites/all/modules/custom
  fi
  if [ ! -z $PARAM_CUSTOM_THEMES_DIR ]; then
    ln -s $PARAM_PROJECT_PATH/$PARAM_CUSTOM_THEMES_DIR $PARAM_PROJECT_PATH_APP/sites/all/themes/custom
  fi
  if [ ! -z $PARAM_CUSTOM_INSTLATION_PROFILE ]; then
    ln -s $PARAM_PROJECT_PATH/$PARAM_CUSTOM_INSTLATION_PROFILE $PARAM_PROJECT_PATH_APP/profiles/
  fi
fi

echo "settings.php file"
if [ -f "$PARAM_PROJECT_PATH_APP/sites/$PARAM_SITE_DIRECTORY/settings.php" ]; then
  chmod 777 "$PARAM_PROJECT_PATH_APP/sites/$PARAM_SITE_DIRECTORY/settings.php"
  rm "$PARAM_PROJECT_PATH_APP/sites/$PARAM_SITE_DIRECTORY/settings.php"
  RESULT=$(($RESULT+$?))
  if [ $RESULT = 1 ]; then
    echo "Error, exiting."
    exit 1
  fi
fi

if [ -e "$PARAM_PROJECT_PATH/conf/local/settings.php" ]
  then
    if [ ! -d "$PARAM_PROJECT_PATH_APP/sites/$PARAM_SITE_DIRECTORY" ]; then
      mkdir "$PARAM_PROJECT_PATH_APP/sites/$PARAM_SITE_DIRECTORY"
    fi
    cp "$PARAM_PROJECT_PATH/conf/local/settings.php" "$PARAM_PROJECT_PATH_APP/sites/$PARAM_SITE_DIRECTORY/settings.php"
  else
    echo "you are missing a settings.php file in ${PARAM_PROJECT_PATH}/conf/local"
    RESULT = 1
fi
RESULT=$(($RESULT+$?))
if [ $RESULT = 1 ]; then
  exit 1
fi

# Create Database.
if [ ! -z $PARAM_DBDUMP_STATUS ]; then
  if [ $PARAM_DBDUMP_STATUS = "ask" ]; then
    until [ "$PARAM_DBDUMP_STATUS" = "yes" ] || [ "$PARAM_DBDUMP_STATUS" = "no" ] || [ "$PARAM_DBDUMP_STATUS" = "y" ] || [ "$PARAM_DBDUMP_STATUS" = "n" ]; do
      echo "Create dump database? (yes or no)"
      read PARAM_DBDUMP_STATUS
    done
  fi
  if [ $PARAM_DBDUMP_STATUS = "yes" ] || [ $PARAM_DBDUMP_STATUS = "y" ]; then
    if [ ! -d "./databases/dump" ]; then
      mkdir "./databases/dump"
    fi
    echo "drush sql-dump";
    drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI sql-dump -y > ./databases/dump/dump-$(date +%Y%m%d%H%M%S).sql
  fi
else
  printf '\n\n\e[1;31m%-6s\e[m\n\n\n' "Please update your local configuration files!"
fi

# Drop database.
if [ ! -z $PARAM_CLEAR_DATABASE ]; then
  echo "PARAM_CLEAR_DATABASE exists in conf.inc"

  if [ $PARAM_CLEAR_DATABASE = "yes" ]; then
    echo "drush sql-drop"
    drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI sql-drop -y
  fi
  if [ $PARAM_CLEAR_DATABASE = "no" ]; then
    echo "== INFO: Build without sql-drop"
  fi
else
  echo "PARAM_CLEAR_DATABASE not exists in conf.inc"
  echo "drush sql-drop"
  drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI sql-drop -y
fi

# Pre build.
source $(dirname $0)"/pre-build.sh"

#Instructions for new project
if [ $PARAM_PROJECT_IS_NEW = "yes" ] || [ $PARAM_PROJECT_IS_NEW = "make" ]; then
  echo "Install site"
  drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI --sites-subdir=$PARAM_SITE_DIRECTORY si $PARAM_INSTALLATION_PROFILE --account-name=$PARAM_ADMIN_NAME --account-pass=$PARAM_ADMIN_PASS --site-name="${PARAM_SITE_NAME}" --site-mail=admin@droptica.com --db-url=$PARAM_DBDRIVER://$PARAM_DBUSER:$PARAM_DBPASS@$PARAM_DBHOST:$PARAM_DBPORT/$PARAM_DBNAME_DRUPAL -y
  echo "drush cc all"
  drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI cc all
fi

#Instructions for existing project
if [ $PARAM_PROJECT_IS_NEW = "no" ];
then

  # Import database.
  if [ ! -z $PARAM_CLEAR_DATABASE ]; then
    echo "PARAM_CLEAR_DATABASE exists in conf.inc"

    if [ $PARAM_CLEAR_DATABASE = "yes" ]; then #Import database
      ######### - move to function
      echo "Import Drupal database $PARAM_DBDUMP_DRUPAL"
      mysql  -u$PARAM_DBUSER -p$PARAM_DBPASS -e "SET autocommit=0;" $PARAM_DBNAME_DRUPAL
      tar xfzO ./databases/$PARAM_DBDUMP_FILE | mysql  -u$PARAM_DBUSER -p$PARAM_DBPASS $PARAM_DBNAME_DRUPAL
      mysql  -u$PARAM_DBUSER -p$PARAM_DBPASS -e "COMMIT;" $PARAM_DBNAME_DRUPAL
      ######### - move to function END
    fi
    if [ $PARAM_CLEAR_DATABASE = "no" ]; then
      echo "== INFO: Build without import database."
    fi
  else
    echo "PARAM_CLEAR_DATABASE not exists in conf.inc"
    ######### - move to function
    echo "Import Drupal database $PARAM_DBDUMP_DRUPAL"
    mysql  -u$PARAM_DBUSER -p$PARAM_DBPASS -e "SET autocommit=0;" $PARAM_DBNAME_DRUPAL
    tar xfzO ./databases/$PARAM_DBDUMP_FILE | mysql  -u$PARAM_DBUSER -p$PARAM_DBPASS $PARAM_DBNAME_DRUPAL
    mysql  -u$PARAM_DBUSER -p$PARAM_DBPASS -e "COMMIT;" $PARAM_DBNAME_DRUPAL
    ######### - move to function END
  fi



  echo "drush updb"
  drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI updb -y
  RESULT=$(($RESULT+$?))
  if [ $RESULT = 1 ]; then
    exit 1
  fi


  # Set admin name and password
  echo "Set admin login: "${PARAM_ADMIN_NAME}
  drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI sql-query "UPDATE users SET name='${PARAM_ADMIN_NAME}' WHERE uid=1"
  echo "Set admin password: "${PARAM_ADMIN_PASS}
  drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI user-password $PARAM_ADMIN_NAME --password="${PARAM_ADMIN_PASS}"
fi

#Delete Files
if [ ! -z $PARAM_DELETE_TXT_FILES ]; then
  if [ $PARAM_DELETE_TXT_FILES = "yes" ]; then
    LIST_DELETE_FILES=("CHANGELOG.txt" "COPYRIGHT.txt" "install.php" "INSTALL.mysql.txt" "INSTALL.pgsql.txt" "INSTALL.sqlite.txt" "INSTALL.txt" "LICENSE.txt" "MAINTAINERS.txt" "README.txt" "UPGRADE.txt" "sites/README.txt" "sites/all/modules/README.txt" "sites/all/themes/README.txt")
    FILE_NAME_COUNT=0
    echo "Delete unnecessary files:"
    for file_name in "${LIST_DELETE_FILES[@]}"; do
      if [ -e "${PARAM_PROJECT_PATH_APP}/${file_name}" ]; then
        rm "${PARAM_PROJECT_PATH_APP}/${file_name}"
        echo "- Delete ${file_name} file"
        DELETE_FILES[FILE_NAME_COUNT]=$file_name
        FILE_NAME_COUNT=$(($FILE_NAME_COUNT + 1))
      fi
    done
    if [ $FILE_NAME_COUNT == 0 ]; then
      echo "- No delete files"
    fi
  fi
fi


# Stage file proxy.
if [ ! -z $PARAM_ENABLE_STAGE_FILE_PROXY ]; then
  echo "PARAM_ENABLE_STAGE_FILE_PROXY exists in conf.inc"

  if [ $PARAM_ENABLE_STAGE_FILE_PROXY = "yes" ]; then
    drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI pm-enable --yes stage_file_proxy
    drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI variable-set stage_file_proxy_origin $PARAM_STAGE_FILE_PROXY_URL
  fi
else
  echo "PARAM_ENABLE_STAGE_FILE_PROXY not exists in conf.inc"
fi


# Post build
source $(dirname $0)"/post-build.sh"


drush -r "$PARAM_PROJECT_PATH_APP" --uri=$PARAM_SITE_URI uli
