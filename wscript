#! /usr/bin/env python
# encoding: utf-8
# Thomas Nagy, 2006-2010 (ita)

# the following two variables are used by the target "waf dist"
VERSION='0.0.1'
APPNAME='algorithms'

# these variables are mandatory ('/' are converted automatically)
top = '.'
out = 'build'

def options(opt):
	opt.load('compiler_cxx qt4 boost')

def configure(conf):
	conf.load('compiler_cxx qt4 boost')
	conf.check(header_name='stdio.h', features='cxx cxxprogram', mandatory=False)
	conf.check_boost(lib='thread', mt=True)
	conf.env.append_value('CXXFLAGS', ['-g'])

def build(bld):
	sources = bld.path.ant_glob('sources/**/*.cpp')
	sources += bld.path.ant_glob('ui/**/*.ui')
	sources += bld.path.ant_glob('qrc/**/*.qrc')
	bld(
				features = 'qt4 cxx cxxprogram',
				uselib   = 'QTCORE QTGUI QTOPENGL QTSVG' 'BOOST_LIBS',
				source   = sources,
				target   = 'prog',
				includes = ["includes", "build"],
				defines  = 'WAF=1', # test
				use = ["BOOST"]
		)
