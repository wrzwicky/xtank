#!/bin/sh -
#
# $Author: lidl $
# $Id: newversion.sh,v 1.2 1991/09/30 00:58:45 lidl Exp $
#
if [ $# = 1 ]; then
    TARGET=$1
else
    TARGET=unknown
fi
if [ ! -r .version ]; then
        /bin/echo 0 > .version
fi
touch .version
v=`cat .version` u=${USER-root} d=`pwd` h=`hostname` t=`date`

FILE=version.c

/bin/echo "char *version1 = \"Xtank UMDENG Release 1.3a\";" > ${FILE}
/bin/echo "char *version2 = \"Build #${v}: (${TARGET}) ${t}\";" >> ${FILE}
/bin/echo "char *version3 = \"${u}@${h}:${d}\";" >> ${FILE}
/bin/echo `expr ${v} + 1` > .version
