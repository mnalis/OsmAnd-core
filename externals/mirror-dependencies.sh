#!/bin/bash

if [ -z "$BASH_VERSION" ]; then
	echo "Invalid shell, re-running using bash..."
	exec bash "$0" "$@"
	exit $?
fi

# Should point to local path, exposed to public as http://builder.osmand.net/dependencies-mirror/
if [ -z "$DEPENDENCIES_MIRROR" ]; then
	DEPENDENCIES_MIRROR=$1
fi
if [ -z "$DEPENDENCIES_MIRROR" ]; then
	echo "DEPENDENCIES_MIRROR is not set!"
	exit 1
fi

curl http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.bz2/download > "$DEPENDENCIES_MIRROR/boost_1_56_0.tar.bz2"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://sourceforge.net/projects/expat/files/expat/2.1.0/expat-2.1.0.tar.gz/download > "$DEPENDENCIES_MIRROR/expat-2.1.0.tar.gz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://sourceforge.net/projects/freetype/files/freetype2/2.5.0/freetype-2.5.0.1.tar.bz2/download > "$DEPENDENCIES_MIRROR/freetype-2.5.0.1.tar.bz2"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://download.osgeo.org/gdal/1.11.1/gdal-1.11.1.tar.gz > "$DEPENDENCIES_MIRROR/gdal-1.11.1.tar.gz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl https://sourceforge.net/projects/glew/files/glew/1.11.0/glew-1.11.0.tgz/download > "$DEPENDENCIES_MIRROR/glew-1.11.0.tgz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://sourceforge.net/projects/ogl-math/files/glm-0.9.5.3/glm-0.9.5.3.zip/download > "$DEPENDENCIES_MIRROR/glm-0.9.5.3.zip"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://download.icu-project.org/files/icu4c/52.1/icu4c-52_1-src.tgz > "$DEPENDENCIES_MIRROR/icu4c-52_1-src.tgz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://www.ijg.org/files/jpegsrc.v9.tar.gz > "$DEPENDENCIES_MIRROR/jpegsrc.v9.tar.gz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://www.libarchive.org/downloads/libarchive-3.1.2.tar.gz > "$DEPENDENCIES_MIRROR/libarchive-3.1.2.tar.gz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://sourceforge.net/projects/libpng/files/libpng16/older-releases/1.6.6/libpng-1.6.6.tar.xz/download > "$DEPENDENCIES_MIRROR/libpng-1.6.6.tar.xz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl https://protobuf.googlecode.com/files/protobuf-2.4.1.tar.bz2 > "$DEPENDENCIES_MIRROR/protobuf-2.4.1.tar.bz2"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi

curl http://zlib.net/zlib-1.2.8.tar.gz > "$DEPENDENCIES_MIRROR/zlib-1.2.8.tar.gz"
exitCode=$?
if [ $exitCode -ne 0 ]; then
	echo "Mirroring failed!"
	exit $exitCode
fi