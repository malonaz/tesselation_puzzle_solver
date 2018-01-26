#!/bin/sh

# Script will install the pre-push hook to run test when pushing to master

SCRIPT_PATH=`dirname $0`

cp $SCRIPT_PATH/pre-push $SCRIPT_PATH/../.git/hooks/pre-push
chmod +x $SCRIPT_PATH/../.git/hooks/pre-push