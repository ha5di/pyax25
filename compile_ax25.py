#! /usr/bin/python

from distutils.core import setup, Extension

module1 = Extension('_ax25', libraries = ['ax25', 'ax25io'], sources = ['_ax25module.c'])

setup (name = 'pyax25',
        version = '1.1',
        author = 'Bela Markus, HA5DI',
        author_email = 'ha5di@hasix.org',
        platforms = ['Linux'],
        url = 'https://sites.google.com/site/dixprs/',
        description = 'Wrapper for LINUX ax.25 stack',
        long_description = 'Wrapper for LINUX ax.25 stack',
        license = 'GPL v2',
        ext_modules = [module1])
