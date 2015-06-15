import string
import os
import sys

# Load project settind
def project_en(name, project_path):
    conf_path = project_path + '/conf/local/envs/' + name + '.inc'

    if not (os.path.isfile(conf_path)):
        message = "\n --------------- WARNING -------------- \n \n" + name + " configuration file not found. \n \n -------------------------------------- \n"
        sys.exit(message)

    return prepare_settings(conf_path)


def project_settings(conf_path):
    if not (os.path.isfile(conf_path)):
        message = "\n --------------- WARNING -------------- \n \n Main configuration file not found.  \n \n -------------------------------------- \n"
        sys.exit(message)

    return prepare_settings(conf_path)


def prepare_settings(file):
    conf = open(file)
    confs = {}
    for line in conf:
        if (line[0] != '#' and line.strip()):
            line.replace('\n', '')
            setting = string.split(line, "=")
            confs[setting[0]] = setting[1].replace('\n', '').replace("\"", '')

    return confs