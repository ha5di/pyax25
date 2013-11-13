#! /usr/bin/python

from distutils.core import setup, Extension

module1 = Extension('_ax25', libraries = ['ax25', 'ax25io'], sources = ['_ax25module.c'])

setup (name = 'PackageName',
        version = '1.0',
        description = 'Wrapping for LINUX ax.25 stack',
        ext_modules = [module1])
        