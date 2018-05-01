# $Id: $
# Maintainer: dino <bassdino@gmx.de>
# Contributor: dino <bassdino@gmx.de>
pkgname=roller_shutter_ctrl
pkgver=0.01
pkgrel=1
pkgdesc="Manager for sever roller shutter motors."
arch=('armv6h' 'armv7h')
url=""
license=('GPL')
groups=()
depends=('wiringpi-git')
makedepends=()
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("")
noextract=()
md5sums=()
validpgpkeys=()


#prepare() {
#   cd "$pkgname-$pkgver"
#   patch -p1 -i "$srcdir/$pkgname-$pkgver.patch"
#}

build() {
#   cd "$pkgname-$pkgver"
   ./configure --prefix=/usr
   make
}

check() {
#   cd "$pkgname-$pkgver"
   make -k check
}

package() {
#   cd "$pkgname-$pkgver"
   make DESTDIR="$pkgdir/" install
}
