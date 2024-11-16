This is port of [the cyberforum demo](https://www.cyberforum.ru/c-beginners/thread1969679.html#post17445881) to SDL3 3.1.6 API and C

Demo in the browser: https://arkanoid-from-ncurses-in-sdl3-c.netlify.app/

To build -DSDL3_DIR must be changed in the `config-win.bat` and `config-web.bat` to your installation path. Install: CMake, MinGW, SDL3, and [emsdk](https://emscripten.org/docs/getting_started/downloads.html).

Building to EXE in CMD:
- config-win
- build-win

Building to WASM in CMD:
- config-web
- build-web
- the `app.js` and `app.wasm` files will be generated in the `public` folder
