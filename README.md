## SDL
Header-only C++23 wrapper for SDL 2.28.

### Building the tests
- Run `bootstrap.[bat|sh]`. This will install doctest + sdl, and create the project under the `./build` directory.
- Run cmake on the `./build` directory and toggle `SDL_BUILD_TESTS`.
- Configure, generate, make.

### Using
The cmake project exports the `sdl::sdl` target, hence you can:
```cmake
find_package         (sdl CONFIG REQUIRED)
target_link_libraries([YOUR_PROJECT_NAME] PRIVATE sdl::sdl)
```
And then point the `sdl_DIR` to the build or installation directory.
Alternatively, you can copy the contents of the include folder to your project.

### Design notes
- Functions:
  - The order of parameters are identical to the SDL with the following exception: 
    - Parameters with default values are moved to the end of the function, and are ordered from the most to least likely to be changed.
- Error Handling:
  - This library does not throw exceptions, except for the exceptions thrown due to standard library usage.
  - Errors are propagated to the caller using `std::expected<T, std::string>`, the unexpected state containing the result of `sdl::error()`.
  - Error handling is performed if the documentation of the native function specifies an error state.
- Versioning:
  - The major, minor and patch version are identical to the supported version of SDL.

### Coverage by headers
- [x] SDL.h
- [x] SDL_assert.h
- [x] SDL_atomic.h
- [ ] SDL_audio.h
- [x] SDL_bits.h
- [x] SDL_blendmode.h
- [x] SDL_clipboard.h
- [ ] ~~SDL_config.h~~                (Reason: Nothing to wrap.)
- [ ] ~~SDL_config.h.cmake~~          (Reason: Nothing to wrap.)
- [ ] ~~SDL_config.h.in~~             (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_android.h~~        (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_emscripten.h~~     (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_iphoneos.h~~       (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_macosx.h~~         (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_minimal.h~~        (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_ngage.h~~          (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_os2.h~~            (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_pandora.h~~        (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_windows.h~~        (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_wingdk.h~~         (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_winrt.h~~          (Reason: Nothing to wrap.)
- [ ] ~~SDL_config_xbox.h~~           (Reason: Nothing to wrap.)
- [ ] ~~SDL_copying.h~~               (Reason: Nothing to wrap.)
- [x] SDL_cpuinfo.h
- [ ] ~~SDL_egl.h~~                   (Reason: Use a dedicated EGL wrapper such as [matus-chochlik/eagine-eglplus](https://github.com/matus-chochlik/eagine-eglplus) instead.)
- [x] SDL_endian.h
- [x] SDL_error.h
- [ ] SDL_events.h
- [x] SDL_filesystem.h
- [ ] SDL_gamecontroller.h
- [x] SDL_gesture.h
- [x] SDL_guid.h
- [ ] SDL_haptic.h
- [x] SDL_hidapi.h
- [x] SDL_hints.h
- [ ] SDL_joystick.h
- [x] SDL_keyboard.h
- [x] SDL_keycode.h
- [x] SDL_loadso.h
- [x] SDL_locale.h
- [x] SDL_log.h
- [x] SDL_main.h
- [x] SDL_messagebox.h
- [x] SDL_metal.h
- [x] SDL_misc.h
- [x] SDL_mouse.h
- [x] SDL_mutex.h
- [ ] ~~SDL_name.h~~                  (Reason: Nothing to wrap.)
- [ ] ~~SDL_opengl.h~~                (Reason: Use a dedicated OpenGL wrapper such as [acdemiralp/gl](https://github.com/acdemiralp/gl) instead.)
- [ ] ~~SDL_opengl_glext.h~~          (Reason: Use a dedicated OpenGL wrapper instead.)
- [ ] ~~SDL_opengles.h~~              (Reason: Use a dedicated OpenGL ES wrapper instead.)
- [ ] ~~SDL_opengles2.h~~             (Reason: Use a dedicated OpenGL ES wrapper instead.)
- [ ] ~~SDL_opengles2_gl2platform.h~~ (Reason: Use a dedicated OpenGL ES wrapper instead.)
- [ ] ~~SDL_opengles2_khrplatform.h~~ (Reason: Use a dedicated OpenGL ES wrapper instead.)
- [ ] SDL_pixels.h
- [x] SDL_platform.h
- [x] SDL_power.h
- [x] SDL_quit.h
- [x] SDL_rect.h
- [ ] SDL_render.h
- [x] SDL_revision.h
- [ ] ~~SDL_revision.h.cmake~~        (Reason: Nothing to wrap.)
- [x] SDL_rwops.h
- [x] SDL_scancode.h
- [x] SDL_sensor.h
- [x] SDL_shape.h
- [ ] SDL_stdinc.h
- [ ] SDL_surface.h
- [ ] SDL_system.h
- [x] SDL_syswm.h
- [ ] ~~SDL_test.h~~                  (Reason: Use a dedicated testing library such as [onqtam/doctest](https://github.com/doctest/doctest) instead.)
- [ ] ~~SDL_test_assert.h~~           (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_common.h~~           (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_compare.h~~          (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_crc32.h~~            (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_font.h~~             (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_fuzzer.h~~           (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_harness.h~~          (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_images.h~~           (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_log.h~~              (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_md5.h~~              (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_memory.h~~           (Reason: Use a dedicated testing library instead.)
- [ ] ~~SDL_test_random.h~~           (Reason: Use a dedicated testing library instead.)
- [x] SDL_thread.h
- [x] SDL_timer.h
- [x] SDL_touch.h
- [ ] ~~SDL_types.h~~                 (Reason: Nothing to wrap.)
- [x] SDL_version.h
- [ ] SDL_video.h
- [x] SDL_vulkan.h
- [ ] ~~begin_code.h~~                (Reason: Nothing to wrap.)
- [ ] ~~close_code.h~~                (Reason: Nothing to wrap.)

### Future work
- Evaluate each use of `std::expected<type, std::string>` and determine if it should be replaced with a `type` with invalid content.
- Consider a detail namespace or separate namespaces for headers to reduce and organize the information presented by intellisense.
- Examples and higher-level abstractions:
  - In particular, a micro application engine consisting of the following abstractions, gathering the content of SDL into several `system` monoliths:
    - `engine`
    - `system`
      - `audio_system`
      - `display_system`
      - `input_system`
      - `rendering_system` 
      - `time_system`
- Tests.