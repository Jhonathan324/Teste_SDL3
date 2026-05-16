#!/usr/bin/env bash
# Compila e instala SDL3, SDL3_ttf e SDL3_image para Linux nativo.
# Executado uma vez durante o build da imagem Docker.
set -e

SDL3_SRC="/tmp/sdl3_src"
SDL3_TTF_SRC="/tmp/sdl3_ttf_src"
SDL3_IMAGE_SRC="/tmp/sdl3_image_src"
SDL3_IMAGE_VERSION="3.2.4"
PREFIX="/usr/local"

# --- SDL3 ---
git clone --depth 1 --branch release-3.4.2 \
    https://github.com/libsdl-org/SDL.git "$SDL3_SRC"
cmake -S "$SDL3_SRC" -B "$SDL3_SRC/build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DSDL_SHARED=ON -DSDL_STATIC=OFF -DSDL_TEST_LIBRARY=OFF \
    -DSDL_X11=ON -DSDL_WAYLAND=OFF
cmake --build "$SDL3_SRC/build" -j"$(nproc)"
cmake --install "$SDL3_SRC/build"

# --- SDL3_ttf ---
git clone --depth 1 --branch release-3.2.2 \
    https://github.com/libsdl-org/SDL_ttf.git "$SDL3_TTF_SRC"
cmake -S "$SDL3_TTF_SRC" -B "$SDL3_TTF_SRC/build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DSDL3TTF_VENDORED=OFF \
    -DSDL3TTF_SAMPLES=OFF
cmake --build "$SDL3_TTF_SRC/build" -j"$(nproc)"
cmake --install "$SDL3_TTF_SRC/build"

# --- SDL3_image ---
curl -fL "https://github.com/libsdl-org/SDL_image/releases/download/release-${SDL3_IMAGE_VERSION}/SDL3_image-${SDL3_IMAGE_VERSION}.tar.gz" \
    -o /tmp/SDL3_image.tar.gz
tar xf /tmp/SDL3_image.tar.gz -C /tmp
cmake -S "/tmp/SDL3_image-${SDL3_IMAGE_VERSION}" -B "$SDL3_IMAGE_SRC" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DSDL3IMAGE_VENDORED=OFF \
    -DSDL3IMAGE_AVIF=OFF -DSDL3IMAGE_BMP=ON  -DSDL3IMAGE_GIF=OFF \
    -DSDL3IMAGE_JPG=OFF  -DSDL3IMAGE_JXL=OFF -DSDL3IMAGE_LBM=OFF \
    -DSDL3IMAGE_PCX=OFF  -DSDL3IMAGE_PNG=ON  -DSDL3IMAGE_PNM=OFF \
    -DSDL3IMAGE_QOI=OFF  -DSDL3IMAGE_SVG=OFF -DSDL3IMAGE_TGA=OFF \
    -DSDL3IMAGE_TIF=OFF  -DSDL3IMAGE_WEBP=OFF -DSDL3IMAGE_XCF=OFF \
    -DSDL3IMAGE_XPM=OFF  -DSDL3IMAGE_XV=OFF \
    -DSDL3IMAGE_SAMPLES=OFF -DSDL3IMAGE_TESTS=OFF
cmake --build "$SDL3_IMAGE_SRC" -j"$(nproc)"
cmake --install "$SDL3_IMAGE_SRC"

# SDL_ttf instala o header em SDL3_ttf/SDL_ttf.h,
# mas o projeto inclui <SDL3/SDL_ttf.h>
ln -sf "$PREFIX/include/SDL3_ttf/SDL_ttf.h" "$PREFIX/include/SDL3/SDL_ttf.h"

# Atualiza cache do linker
ldconfig

# Limpeza
rm -rf "$SDL3_SRC" "$SDL3_TTF_SRC" "$SDL3_IMAGE_SRC" \
       /tmp/SDL3_image.tar.gz "/tmp/SDL3_image-${SDL3_IMAGE_VERSION}"

echo "SDL3, SDL3_ttf e SDL3_image instalados em $PREFIX"
