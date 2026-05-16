FROM mcr.microsoft.com/devcontainers/base:2.0.4-noble

# Dependências de build
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
        cmake \
        git \
        python3 \
        xz-utils \
    && rm -rf /var/lib/apt/lists/*

# Emscripten via emsdk (versão latest)
RUN git clone https://github.com/emscripten-core/emsdk.git /opt/emsdk \
    && /opt/emsdk/emsdk install latest \
    && /opt/emsdk/emsdk activate latest \
    && chmod -R a+rX /opt/emsdk/upstream/emscripten /opt/emsdk/node

# Disponibiliza emcc no PATH para todos os usuários
RUN echo 'source /opt/emsdk/emsdk_env.sh' >> /etc/bash.bashrc

# Pré-compila ports SDL3 e SDL3_ttf para WebAssembly (cache no build da imagem)
RUN bash -c "source /opt/emsdk/emsdk_env.sh 2>/dev/null \
    && echo 'int main(){}' | emcc -x c - -sUSE_SDL=3 -sUSE_SDL_TTF=3 -o /tmp/warmup.html \
    && rm -f /tmp/warmup.*"

# SDL_ttf instala o header em SDL3_ttf/SDL_ttf.h, mas o projeto inclui SDL3/SDL_ttf.h
RUN ln -sf /opt/emsdk/upstream/emscripten/cache/sysroot/include/SDL3_ttf/SDL_ttf.h \
           /opt/emsdk/upstream/emscripten/cache/sysroot/include/SDL3/SDL_ttf.h

# Compila SDL3_image para WebAssembly e instala no sysroot do Emscripten
COPY prebuild-sdl3image.sh /tmp/prebuild-sdl3image.sh
RUN bash /tmp/prebuild-sdl3image.sh && rm /tmp/prebuild-sdl3image.sh
