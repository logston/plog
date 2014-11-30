#! /bin/sh

rm -rf /srv/plog/plog_src/*
cp -R $WORKSPACE/* /srv/plog/plog_src/
. /srv/plog/bin/activate
cd /srv/plog/plog_src
git clone https://github.com/xdissent/jekyll-rst.git _plugins/jekyll-rst
jekyll build
