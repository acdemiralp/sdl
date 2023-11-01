## SDL
Header-only C++20 wrapper for SDL 2.28.4+.

### Building the tests
- Run `bootstrap.[bat|sh]`. This will install doctest + sdl, and create the project under the `./build` directory.
- Run cmake on the `./build` directory and toggle `BUILD_TESTS`.
- Configure, generate, make.

### Using
The cmake project exports the `sdl::sdl` target, hence you can:
```cmake
find_package         (sdl CONFIG REQUIRED)
target_link_libraries([YOUR_PROJECT_NAME] PRIVATE sdl::sdl)
```
And then point the `sdl_DIR` to the build or installation directory.
Alternatively, you can copy the contents of the include folder to your project.

### Usage Notes
- Define `SDL_USE_EXCEPTIONS` to check the return values of all viable functions and throw an exception on failure.

### Design notes
- Constructors:
  - Copy constructors are deleted unless SDL provides copy functions for the object.
  - Move constructors are available whenever possible.
  - The object wrappers have the capability to both:
    - Construct a  new      SDL object, and be           responsible for its destruction.
    - Accept    an existing SDL object, and be or not be responsible for its destruction.
- Structures:
  - Structs are for POD data types. They may only contain member variables, constructors, destructors and assignment operators.
- Statics:
  - Namespace functions are preferred over static member functions whenever possible.
- Error Handling:
  - Error handling is performed only if the documentation of the native function specifies an error state.
- Versioning:
  - The major, minor and patch version are identical to the supported version of SDL.
- Other:
  - The order of parameters are identical to the SDL with the following exceptions: 
    - Parameters with default values are moved to the end of the function.
    - Parameters with default values are ordered from the most to least likely to be changed.

### Coverage 
- The coverage is derived from the [SDL include directory](https://github.com/libsdl-org/SDL/blob/SDL2/include) and categorized according to the [SDL 2.0 API by Category](https://wiki.libsdl.org/SDL2/APIByCategory) documentation.
- Categories:
  - Basics:
    - [x] Initialization (SDL.h):
      - [x] Functions:
        - [ ] ~~SDL_Init~~ (Reason: Deprecated in favor of SDL_InitSubSystem.)
        - [x] SDL_InitSubSystem
        - [x] SDL_QuitSubSystem
        - [x] SDL_WasInit
        - [x] SDL_Quit
    - [ ] ~~Assertions (SDL_assert)~~ (Reason: Use standard assertions i.e. `<cassert>` instead.)
    - [x] Error (SDL_error.h):
      - [x] Enums:
        - [ ] ~~SDL_errorcode~~ (Reason: Internal enum.)
      - [x] Functions:
        - [x] SDL_SetError
        - [x] SDL_GetError
        - [x] SDL_GetErrorMsg
        - [x] SDL_ClearError
        - [ ] ~~SDL_Error~~ (Reason: Internal function.)
    - [x] Log (SDL_log.h):
      - [x] Enums:
        - [x] SDL_LogCategory
        - [x] SDL_LogPriority
      - [x] Functions: 
        - [x] SDL_LogSetAllPriority
        - [x] SDL_LogSetPriority
        - [x] SDL_LogGetPriority
        - [x] SDL_LogResetPriorities
        - [x] SDL_Log
        - [x] SDL_LogVerbose
        - [x] SDL_LogDebug
        - [x] SDL_LogInfo
        - [x] SDL_LogWarn
        - [x] SDL_LogError
        - [x] SDL_LogCritical
        - [x] SDL_LogMessage
        - [x] SDL_LogMessageV
        - [x] SDL_LogGetOutputFunction
        - [x] SDL_LogSetOutputFunction
    - [x] Version (SDL_version.h):
      - [x] Functions:
        - [x] SDL_GetVersion
        - [x] SDL_GetRevision
        - [ ] ~~SDL_GetRevisionNumber~~ (Reason: Deprecated in favor of SDL_GetRevision.)
  - Video:
    - [x] Clipboard (SDL_clipboard.h):
      - [x] SDL_GetClipboardText
      - [x] SDL_SetClipboardText
      - [x] SDL_HasClipboardText
      - [x] SDL_GetPrimarySelectionText
      - [x] SDL_SetPrimarySelectionText
      - [x] SDL_HasPrimarySelectionText
  - Input Events:
  - Force Feedback:
  - Audio:
  - Threads:
  - [x] Timers (SDL_timer.h):
    - [x] Functions:
      - [ ] ~~SDL_GetTicks~~ (Reason: Deprecated in favor of SDL_GetTicks64.)
      - [x] SDL_GetTicks64
      - [x] SDL_GetPerformanceCounter
      - [x] SDL_GetPerformanceFrequency
      - [x] SDL_Delay
      - [x] SDL_AddTimer
      - [x] SDL_RemoveTimer
    - [x] Macros:
      - [ ] ~~SDL_TICKS_PASSED~~ (Reason: Intended to be used with SDL_GetTicks, which is removed.)
  - File Abstraction:
  - [x] Shared Object Support (SDL_loadso.h):
    - [x] Functions:
      - [x] SDL_LoadObject 
      - [x] SDL_UnloadObject
      - [x] SDL_LoadFunction
  - [ ] Platform and CPU Information:
    - [x] Platform Detection (SDL_platform.h):
      - [x] Functions:
        - [x] SDL_GetPlatform
    - [x] CPU Feature Detection (SDL_cpuinfo.h):
      - [x] Functions: 
        - [x] SDL_GetCPUCount
        - [x] SDL_GetCPUCacheLineSize
        - [x] SDL_HasRDTSC
        - [x] SDL_HasAltiVec
        - [x] SDL_HasMMX
        - [x] SDL_Has3DNow
        - [x] SDL_HasSSE
        - [x] SDL_HasSSE2
        - [x] SDL_HasSSE3
        - [x] SDL_HasSSE41
        - [x] SDL_HasSSE42
        - [x] SDL_HasAVX
        - [x] SDL_HasAVX2
        - [x] SDL_HasAVX512F
        - [x] SDL_HasARMSIMD
        - [x] SDL_HasNEON
        - [x] SDL_HasLSX
        - [x] SDL_HasLASX
        - [x] SDL_GetSystemRAM
        - [x] SDL_SIMDGetAlignment
        - [x] SDL_SIMDAlloc
        - [x] SDL_SIMDRealloc
        - [x] SDL_SIMDFree
    - [ ] Byte Order and Byte Swapping (SDL_endian.h):
      - [ ] Functions: 
        - [ ] 
    - [x] Bit Manipulation (SDL_bits.h):
      - [x] Functions: 
        - [x] SDL_MostSignificantBitIndex32
        - [x] SDL_HasExactlyOneBitSet32
  - [x] Power Management (SDL_power.h):
    - [x] Enums:
      - [x] SDL_PowerState
    - [x] Functions:
      - [x] SDL_GetPowerInfo
  - [ ] Additional Functionality:
    - [x] Message Boxes (SDL_messagebox.h):
      - [x] Classes / Structs:
        - [x] SDL_MessageBoxButtonData
        - [ ] ~~SDL_MessageBoxColor~~ (Reason: Use `std::array<std::uint8_t, 3>` instead.)
        - [x] SDL_MessageBoxColorScheme
        - [ ] ~~SDL_MessageBoxData~~ (Reason: Unnecessary as function `show_message_box` accepts the struct members as parameters.)
      - [x] Enums:
        - [ ] ~~SDL_MessageBoxButtonFlags~~ (Reason: Unnecessary as struct `message_box_button_data` contains variables for each flag.)
        - [ ] ~~SDL_MessageBoxColorType~~ (Reason: Unnecessary as struct `message_box_color_scheme` contains variables for each type.)
        - [x] SDL_MessageBoxFlags
      - [x] Functions:
        - [x] SDL_ShowMessageBox
        - [x] SDL_ShowSimpleMessageBox 
    - [ ] Platform-Specific Functionality (SDL_system.h):
    - [ ] Standard Library Functionality (SDL_stdinc.h):
