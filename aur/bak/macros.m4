divert(-1)
define(`NOBRACKS', `translit($1, `[]', `')')dnl
define(`AC_INIT', `define(`VERSION', `NOBRACKS(`$2')')')dnl defines the value for pkgver
include(`../configure.ac')
define(`TARBALL', `neoleo-VERSION.tar.gz')
define(`MD5SUM', `esyscmd(md5sum ../TARBALL | cut -d " " -f 1 | tr -d "\n")')
dnl define(`MD5SUM', `MD5SUM_CMD')

define(`LQ',`changequote(<,>)`dnl'
changequote`'')
define(`RQ',`changequote(<,>)dnl`
'changequote`'')

define(`BASE_DEPENDS', `depends=(RQ()`ncurses'RQ()  RQ()`libxt'RQ())')
define(`CHROOT_DEPENDS', `BASE_DEPENDS')
define(`NONCHROOT_DEPENDS', `BASE_DEPENDS')
ifdef(`MKCHROOT', `define(`DEPENDS', `CHROOT_DEPENDS')', `define(`DEPENDS', `NONCHROOT_DEPENDS')')

define(`RELEASE_SOURCE', `source=("https://github.com/blippy/neoleo/releases/download/v$pkgver/$pkgname-$pkgver.tar.gz")')
define(`NONRELEASE_SOURCE', `source=("file://TARBALL")')
ifdef(`MKRELEASE', `define(`SOURCE', `RELEASE_SOURCE')', `define(`SOURCE', `NONRELEASE_SOURCE')')

ifdef(`MKCHROOT', `define(`CONFX', `--without-motif')', `define(`CONFX', `--without-motif')')
divert(0)dnl
