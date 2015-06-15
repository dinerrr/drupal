import os
import sys
import datetime

from fabric.api import *

import droptica_fabs.utils

############ GENERAL SETTINGS #################

# Load users ~/.ssh/config
# With this enabled your ssh settings in your .ssh/conf will be available
env.use_ssh_config = True

############ PROJECT Settings #################

# Load conf/local/conf.inc and make all settings available available to all tasks as conf["PARAM_KEY"]

 # set up local dir and add it to path.
scripts_path = os.path.abspath(os.path.dirname(__file__))
if not scripts_path in sys.path:
    sys.path.insert(1, scripts_path)

# PROJECT DIR
project_path= os.path.abspath(os.path.join(scripts_path, os.pardir))
conf_path = project_path + '/conf/local/conf.inc'

# Load general project configuration
conf = droptica_fabs.utils.project_settings(conf_path)

# get those used in scripts but not comming from conf
conf["PARAM_PROJECT_PATH"] = project_path
conf["PARAM_PROJECT_PATH_APP"] = project_path + '/' + conf["PARAM_DRUPAL_LOCATION"]



############ EVN SPECS #################

# This is to be called before the actual fab task for remove servers. This will load config from
# conf/local/envs/NAME.inc and will set up your enviromnent. Without this you only have local
#
# To do task on some server you call 'fab en:name task'
#
# param {name} - name of env equal to name of file.

def en(name):
    if name == None:
        sys.exit("No environment defined")
    else:
        conf = droptica_fabs.utils.project_en(name, project_path)
        env.hosts = conf["PARAM_HOST"]
        env.remotes = conf

# alias for en:dev
def dev():
    en('dev')

# alias for en:prod
def prod():
    en('prod')


############## TASKS ###############

def get_db():
    path = env.remotes["PARAM_DEV_FILES_LOCATION"] + '/' + env.remotes["PARAM_DEV_DB_NAME"]
    dest = project_path + '/databases/' + conf["PARAM_DBDUMP_FILE"]

    print 'Copying file {} from remote to {} at local'.format(path, dest)

    get(path, dest)
    print 'done with this!'


