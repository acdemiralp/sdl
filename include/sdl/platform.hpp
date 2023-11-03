#pragma once

#include <string>

#include <SDL_platform.h>

namespace sdl
{
[[nodiscard]]
inline std::string get_platform_string()
{
  return SDL_GetPlatform();
}

// Conveniences.

enum class platform
{
  aix                 ,
  android             ,
  bsdi                ,
  dreamcast           ,
  emscripten          ,
  freebsd             ,
  haiku               ,
  hpux                ,
  irix                ,
  linux               ,
  atari_mint          ,
  macos               ,
  nacl                ,
  netbsd              ,
  openbsd             ,
  os2                 ,
  osf                 ,
  qnx_neutrino        ,
  risc_os             ,
  solaris             ,
  windows             ,
  winrt               ,
  wingdk              ,
  xbox_one            ,
  xbox_series_xs      ,
  ios                 ,
  tvos                ,
  playstation_2       ,
  playstation_portable,
  playstation_vita    ,
  nokia_ngage         ,
  nintendo_3ds        ,
  managarm            ,
  unknown
};

[[nodiscard]]
inline platform    get_platform       ()
{
  const auto string = get_platform_string();

  if (string == "AIX")                  return platform::aix;
  if (string == "Android")              return platform::android;
  if (string == "BSDI")                 return platform::bsdi;
  if (string == "Dreamcast")            return platform::dreamcast;
  if (string == "Emscripten")           return platform::emscripten;
  if (string == "FreeBSD")              return platform::freebsd;
  if (string == "Haiku")                return platform::haiku;
  if (string == "HP-UX")                return platform::hpux;
  if (string == "Irix")                 return platform::irix;
  if (string == "Linux")                return platform::linux;
  if (string == "Atari MiNT")           return platform::atari_mint;
  if (string == "macOS")                return platform::macos;
  if (string == "NaCl")                 return platform::nacl;
  if (string == "NetBSD")               return platform::netbsd;
  if (string == "OpenBSD")              return platform::openbsd;
  if (string == "OS/2")                 return platform::os2;
  if (string == "OSF/1")                return platform::osf;
  if (string == "QNX Neutrino")         return platform::qnx_neutrino;
  if (string == "RISC OS")              return platform::risc_os;
  if (string == "Solaris")              return platform::solaris;
  if (string == "Windows")              return platform::windows;
  if (string == "WinRT")                return platform::winrt;
  if (string == "WinGDK")               return platform::wingdk;
  if (string == "Xbox One")             return platform::xbox_one;
  if (string == "Xbox Series X|S")      return platform::xbox_series_xs;
  if (string == "iOS")                  return platform::ios;
  if (string == "tvOS")                 return platform::tvos;
  if (string == "PlayStation 2")        return platform::playstation_2;
  if (string == "PlayStation Portable") return platform::playstation_portable;
  if (string == "PlayStation Vita")     return platform::playstation_vita;
  if (string == "Nokia N-Gage")         return platform::nokia_ngage;
  if (string == "Nintendo 3DS")         return platform::nintendo_3ds;
  if (string == "Managarm")             return platform::managarm;

  return platform::unknown;
}
}