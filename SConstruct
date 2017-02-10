import os

cc = 'gcc'

env = Environment(
	CC = cc,
	ENV = os.environ,
	SHLIBPREFIX='',
	LIBS=['m', 'dl', 'ev', 'lua'],
	CFLAGS = [
		'-g',
		'-I.',
		'-Wall',
		'-Wno-unused-value',
		'-Wno-comment',
		'-I/usr/include/lua5.1',
	]
)

if os.environ.has_key('CC'):
  env.Replace(CC = os.environ['CC'])

lua_cori_core = env.SharedLibrary('cori_core.so', [
		'lua-cori.c',
		'tap.c',
	],
)
