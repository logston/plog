#! /bin/sh

# Get newest jekyll-rst
git submodule foreach git pull origin master

# Build new source
. /srv/plog/bin/activate
jekyll build

# Move files into place
rm -rf /srv/plog/plog_src/*
cp -R $WORKSPACE/* /srv/plog/plog_src/
