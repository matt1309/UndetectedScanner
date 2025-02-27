sudo apt-get install -y git ninja-build cmake curl zlib1g-dev

# The following are needed because we are going to change some autoconf scripts,
# both for libnghttp2 and curl.
sudo apt-get install -y autoconf automake autotools-dev pkg-config libtool

# Dependencies for downloading and building nghttp2
sudo apt-get install -y bzip2

# Dependencies for downloading and building curl
sudo apt-get install -y xz-utils

# Dependencies for building libnss
# See https://firefox-source-docs.mozilla.org/security/nss/build.html#mozilla-projects-nss-building
sudo apt-get install -y g++ golang-go unzip



BASEDIR=$(dirname $0)

cd ${BASEDIR}


BASEDIR=${PWD}

cd BASEDIR=${PWD}

# Download and compile libbrotli
BROTLI_VERSION=1.1.0
curl -L https://github.com/google/brotli/archive/refs/tags/v${BROTLI_VERSION}.tar.gz -o brotli-${BROTLI_VERSION}.tar.gz && \
tar xf brotli-${BROTLI_VERSION}.tar.gz
cd brotli-${BROTLI_VERSION} && \
mkdir build && cd build && \
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./installed .. && \
cmake --build . --config Release --target install

cd BASEDIR=${PWD}

BORING_SSL_COMMIT=d24a38200fef19150eef00cad35b138936c08767
curl -L https://github.com/google/boringssl/archive/${BORING_SSL_COMMIT}.zip -o boringssl.zip && \
unzip boringssl && \
    mv boringssl-${BORING_SSL_COMMIT} boringss
    
    
 cp patches/boringssl-*.patch boringssl/
 cd boringssl && \
 for p in $(ls boringssl-*.patch); do patch -p1 < $p; done && \
    mkdir build && cd build && \
    cmake \
        -DCMAKE_C_FLAGS="-Wno-error=array-bounds -Wno-error=stringop-overflow" \
        -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE=on -GNinja .. && \
    ninja

mkdir boringssl/build/lib && \
ln -s ../crypto/libcrypto.a boringssl/build/lib/libcrypto.a && \
    ln -s ../ssl/libssl.a boringssl/build/lib/libssl.a && \
    cp -R boringssl/include boringssl/build


cd ${BASEDIR}

NGHTTP2_VERSION=nghttp2-1.63.0
NGHTTP2_URL=https://github.com/nghttp2/nghttp2/releases/download/v1.63.0/nghttp2-1.63.0.tar.bz2

curl -o ${NGHTTP2_VERSION}.tar.bz2 -L ${NGHTTP2_URL}
tar xf ${NGHTTP2_VERSION}.tar.bz2

# Compile nghttp2
cd ${NGHTTP2_VERSION} && \
 ./configure --prefix=${BASEDIR}/build/${NGHTTP2_VERSION}/installed --with-pic --disable-shared && \
 make && make install
 
 
cd ${BASEDIR}
# Download curl.
CURL_VERSION=curl-8.7.1
curl -o ${CURL_VERSION}.tar.xz https://curl.se/download/${CURL_VERSION}.tar.xz
tar xf ${CURL_VERSION}.tar.xz

# Patch curl and re-generate the configure script
cp ${BASEDIR}/patches/curl-*.patch ${CURL_VERSION}/
cd ${CURL_VERSION} && \
for p in $(ls curl-*.patch); do patch -p1 < $p; done && \
autoreconf -fi

cd ${BASEDIR}
# Compile curl with nghttp2, libbrotli and nss (firefox) or boringssl (chrome).
# Enable keylogfile for debugging of TLS traffic.
cd ${CURL_VERSION} && \
    ./configure --prefix=${BASEDIR}/build/install \
                --enable-static \
                --disable-shared \
                --enable-websockets \
                --with-nghttp2=${BASEDIR}/build/${NGHTTP2_VERSION}/installed \
                --with-brotli=${BASEDIR}/brotli-${BROTLI_VERSION}/build/installed \
                --with-openssl=${BASEDIR}/build/boringssl/build \
                LIBS="-pthread" \
                CFLAGS="-I${BASEDIR}/build/boringssl/build" \
                USE_CURL_SSLKEYLOGFILE=true && \
    make && make install

cd ${BASEDIR}
mkdir out && \
    cp ${BASEDIR}/build/install/bin/curl-impersonate-chrome out/ && \
    ln -s curl-impersonate-chrome out/curl-impersonate && \
    strip out/curl-impersonate

# Verify that the resulting 'curl' has all the necessary features.
./out/curl-impersonate -V | grep -q zlib && \
    ./out/curl-impersonate -V | grep -q brotli && \
    ./out/curl-impersonate -V | grep -q nghttp2 && \
    ./out/curl-impersonate -V | grep -q -e NSS -e BoringSSL



# Copy libcurl-impersonate and symbolic links
#this needs to be updated to put the files in the lib/include folders accordingly.
cp -d ${BASEDIR}/build/install/lib/libcurl-impersonate* ${BASEDIR}/out

