#!/usr/bin/env bash
# Compila SDL3_image para WebAssembly e instala no sysroot do Emscripten.
# Executado uma vez durante o build da imagem Docker.
set -e

source /opt/emsdk/emsdk_env.sh 2>/dev/null

SDL3_IMAGE_VERSION="3.2.4"
WORKDIR="/tmp/sdl3_image_build"

# Baixa SDL3_image
curl -fL "https://github.com/libsdl-org/SDL_image/releases/download/release-${SDL3_IMAGE_VERSION}/SDL3_image-${SDL3_IMAGE_VERSION}.tar.gz" \
    -o /tmp/SDL3_image.tar.gz
tar xf /tmp/SDL3_image.tar.gz -C /tmp

# Adiciona versão ao cmake config do port SDL3 (necessário para SDL3_image encontrá-lo)
SYSROOT_CMAKE="/opt/emsdk/upstream/emscripten/cache/sysroot/lib/cmake/SDL3"
mkdir -p "$SYSROOT_CMAKE"
cat > "$SYSROOT_CMAKE/sdl3-config-version.cmake" << 'EOF'
set(PACKAGE_VERSION "3.4.2")
if(PACKAGE_VERSION VERSION_LESS PACKAGE_FIND_VERSION)
  set(PACKAGE_VERSION_COMPATIBLE FALSE)
else()
  set(PACKAGE_VERSION_COMPATIBLE TRUE)
  if(PACKAGE_FIND_VERSION STREQUAL PACKAGE_VERSION)
    set(PACKAGE_VERSION_EXACT TRUE)
  endif()
endif()
EOF

# Compila SDL3_image com suporte a PNG e BMP apenas
mkdir -p "$WORKDIR"
emcmake cmake "/tmp/SDL3_image-${SDL3_IMAGE_VERSION}" \
    -B "$WORKDIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL3IMAGE_VENDORED=ON \
    -DSDL3IMAGE_AVIF=OFF -DSDL3IMAGE_BMP=ON  -DSDL3IMAGE_GIF=OFF \
    -DSDL3IMAGE_JPG=OFF  -DSDL3IMAGE_JXL=OFF -DSDL3IMAGE_LBM=OFF \
    -DSDL3IMAGE_PCX=OFF  -DSDL3IMAGE_PNG=ON  -DSDL3IMAGE_PNM=OFF \
    -DSDL3IMAGE_QOI=OFF  -DSDL3IMAGE_SVG=OFF -DSDL3IMAGE_TGA=OFF \
    -DSDL3IMAGE_TIF=OFF  -DSDL3IMAGE_WEBP=OFF -DSDL3IMAGE_XCF=OFF \
    -DSDL3IMAGE_XPM=OFF  -DSDL3IMAGE_XV=OFF \
    -DSDL3IMAGE_SAMPLES=OFF -DSDL3IMAGE_TESTS=OFF \
    -DSDL3_DIR="$SYSROOT_CMAKE"

cmake --build "$WORKDIR" -j"$(nproc)"

# Instala no sysroot
SYSROOT_LIB="/opt/emsdk/upstream/emscripten/cache/sysroot/lib/wasm32-emscripten"
SYSROOT_INC="/opt/emsdk/upstream/emscripten/cache/sysroot/include"
cp "$WORKDIR/libSDL3_image.a" "$SYSROOT_LIB/"
cp -r "/tmp/SDL3_image-${SDL3_IMAGE_VERSION}/include/SDL3_image" "$SYSROOT_INC/"

# Limpeza
rm -rf "$WORKDIR" /tmp/SDL3_image.tar.gz "/tmp/SDL3_image-${SDL3_IMAGE_VERSION}"

echo "SDL3_image ${SDL3_IMAGE_VERSION} instalado para WebAssembly."
