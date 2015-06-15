#dev server


###
# PARAM_HOST
#
# you can have hosts set up so
# PARAM_HOST=user@server:port
#
# your server definitions from ~/.ssh/conf wil also work
####

PARAM_HOST=user@server:port

###
# Enviromnens setting
#
# The belo can differ depending on env type you want to use. this is an example
# for set up of dev droptica servers. Productions can have other strings.
#
####

# location of the project files on the remote host
PARAM_DEV_FILES_LOCATION=/home/projects_db_and_files/hotron

PARAM_DEV_DB_NAME=database.sql.tar.gz
