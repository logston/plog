#!/usr/bin/env python
# -*- coding: utf-8 -*- #
from __future__ import unicode_literals
import os

AUTHOR = 'Paul Logston'
SITENAME = 'plog'

PATH = 'content'

SITE_EMAIL = ''

TIMEZONE = 'UTC'

DEFAULT_LANG = 'en'

DEFAULT_PAGINATION = 10

ARTICLE_URL = ARTICLE_SAVE_AS = '{slug}.html'
PAGE_URL = PAGE_SAVE_AS = 'pages/{slug}.html'

# Uncomment following line if you want document-relative URLs when developing
RELATIVE_URLS = True

DELETE_OUTPUT_DIRECTORY = True

THEME = 'themes/basic/'

GITHUB_URL = 'https://github.com/logston/plog'
GITHUB_USERNAME = 'logston'

TWITTER_HANDLE = 'paullogston'

SITE_DESCRIPTION = 'Code, biology, etc. Hope you enjoy :)'

STATIC_PATHS = [
    'images',
    'legacy',
    'extra/robots.txt',
    'extra/CNAME',
]

EXTRA_PATH_METADATA = {
    'extra/CNAME': {'path': 'CNAME'},
    'extra/robots.txt': {'path': 'robots.txt'},
}

