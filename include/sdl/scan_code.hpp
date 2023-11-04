#pragma once

#include <cstddef>

#include <SDL_scancode.h>

namespace sdl
{
enum class scan_code
{
  unknown                      = SDL_SCANCODE_UNKNOWN,
  a                            = SDL_SCANCODE_A,
  b                            = SDL_SCANCODE_B,
  c                            = SDL_SCANCODE_C,
  d                            = SDL_SCANCODE_D,
  e                            = SDL_SCANCODE_E,
  f                            = SDL_SCANCODE_F,
  g                            = SDL_SCANCODE_G,
  h                            = SDL_SCANCODE_H,
  i                            = SDL_SCANCODE_I,
  j                            = SDL_SCANCODE_J,
  k                            = SDL_SCANCODE_K,
  l                            = SDL_SCANCODE_L,
  m                            = SDL_SCANCODE_M,
  n                            = SDL_SCANCODE_N,
  o                            = SDL_SCANCODE_O,
  p                            = SDL_SCANCODE_P,
  q                            = SDL_SCANCODE_Q,
  r                            = SDL_SCANCODE_R,
  s                            = SDL_SCANCODE_S,
  t                            = SDL_SCANCODE_T,
  u                            = SDL_SCANCODE_U,
  v                            = SDL_SCANCODE_V,
  w                            = SDL_SCANCODE_W,
  x                            = SDL_SCANCODE_X,
  y                            = SDL_SCANCODE_Y,
  z                            = SDL_SCANCODE_Z,
  _1                           = SDL_SCANCODE_1,
  _2                           = SDL_SCANCODE_2,
  _3                           = SDL_SCANCODE_3,
  _4                           = SDL_SCANCODE_4,
  _5                           = SDL_SCANCODE_5,
  _6                           = SDL_SCANCODE_6,
  _7                           = SDL_SCANCODE_7,
  _8                           = SDL_SCANCODE_8,
  _9                           = SDL_SCANCODE_9,
  _0                           = SDL_SCANCODE_0,
  _return                      = SDL_SCANCODE_RETURN,
  escape                       = SDL_SCANCODE_ESCAPE,
  backspace                    = SDL_SCANCODE_BACKSPACE,
  tab                          = SDL_SCANCODE_TAB,
  space                        = SDL_SCANCODE_SPACE,
  minus                        = SDL_SCANCODE_MINUS,
  equals                       = SDL_SCANCODE_EQUALS,
  left_bracket                 = SDL_SCANCODE_LEFTBRACKET,
  right_bracket                = SDL_SCANCODE_RIGHTBRACKET,
  backslash                    = SDL_SCANCODE_BACKSLASH,
  non_us_hash                  = SDL_SCANCODE_NONUSHASH,
  semicolon                    = SDL_SCANCODE_SEMICOLON,
  apostrophe                   = SDL_SCANCODE_APOSTROPHE,
  grave                        = SDL_SCANCODE_GRAVE,
  comma                        = SDL_SCANCODE_COMMA,
  period                       = SDL_SCANCODE_PERIOD,
  slash                        = SDL_SCANCODE_SLASH,
  caps_lock                    = SDL_SCANCODE_CAPSLOCK,
  f1                           = SDL_SCANCODE_F1,
  f2                           = SDL_SCANCODE_F2,
  f3                           = SDL_SCANCODE_F3,
  f4                           = SDL_SCANCODE_F4,
  f5                           = SDL_SCANCODE_F5,
  f6                           = SDL_SCANCODE_F6,
  f7                           = SDL_SCANCODE_F7,
  f8                           = SDL_SCANCODE_F8,
  f9                           = SDL_SCANCODE_F9,
  f10                          = SDL_SCANCODE_F10,
  f11                          = SDL_SCANCODE_F11,
  f12                          = SDL_SCANCODE_F12,
  print_screen                 = SDL_SCANCODE_PRINTSCREEN,
  scroll_lock                  = SDL_SCANCODE_SCROLLLOCK,
  pause                        = SDL_SCANCODE_PAUSE,
  insert                       = SDL_SCANCODE_INSERT,
  home                         = SDL_SCANCODE_HOME,
  page_up                      = SDL_SCANCODE_PAGEUP,
  _delete                      = SDL_SCANCODE_DELETE,
  end                          = SDL_SCANCODE_END,
  page_down                    = SDL_SCANCODE_PAGEDOWN,
  right                        = SDL_SCANCODE_RIGHT,
  left                         = SDL_SCANCODE_LEFT,
  down                         = SDL_SCANCODE_DOWN,
  up                           = SDL_SCANCODE_UP,
  num_lock_clear               = SDL_SCANCODE_NUMLOCKCLEAR,
  keypad_divide                = SDL_SCANCODE_KP_DIVIDE,
  keypad_multiply              = SDL_SCANCODE_KP_MULTIPLY,
  keypad_minus                 = SDL_SCANCODE_KP_MINUS,
  keypad_plus                  = SDL_SCANCODE_KP_PLUS,
  keypad_enter                 = SDL_SCANCODE_KP_ENTER,
  keypad_1                     = SDL_SCANCODE_KP_1,
  keypad_2                     = SDL_SCANCODE_KP_2,
  keypad_3                     = SDL_SCANCODE_KP_3,
  keypad_4                     = SDL_SCANCODE_KP_4,
  keypad_5                     = SDL_SCANCODE_KP_5,
  keypad_6                     = SDL_SCANCODE_KP_6,
  keypad_7                     = SDL_SCANCODE_KP_7,
  keypad_8                     = SDL_SCANCODE_KP_8,
  keypad_9                     = SDL_SCANCODE_KP_9,
  keypad_0                     = SDL_SCANCODE_KP_0,
  keypad_period                = SDL_SCANCODE_KP_PERIOD,
  non_us_backslash             = SDL_SCANCODE_NONUSBACKSLASH,
  application                  = SDL_SCANCODE_APPLICATION,
  power                        = SDL_SCANCODE_POWER,
  keypad_equals                = SDL_SCANCODE_KP_EQUALS,
  f13                          = SDL_SCANCODE_F13,
  f14                          = SDL_SCANCODE_F14,
  f15                          = SDL_SCANCODE_F15,
  f16                          = SDL_SCANCODE_F16,
  f17                          = SDL_SCANCODE_F17,
  f18                          = SDL_SCANCODE_F18,
  f19                          = SDL_SCANCODE_F19,
  f20                          = SDL_SCANCODE_F20,
  f21                          = SDL_SCANCODE_F21,
  f22                          = SDL_SCANCODE_F22,
  f23                          = SDL_SCANCODE_F23,
  f24                          = SDL_SCANCODE_F24,
  execute                      = SDL_SCANCODE_EXECUTE,
  help                         = SDL_SCANCODE_HELP,
  menu                         = SDL_SCANCODE_MENU,
  select                       = SDL_SCANCODE_SELECT,
  stop                         = SDL_SCANCODE_STOP,
  again                        = SDL_SCANCODE_AGAIN,
  undo                         = SDL_SCANCODE_UNDO,
  cut                          = SDL_SCANCODE_CUT,
  copy                         = SDL_SCANCODE_COPY,
  paste                        = SDL_SCANCODE_PASTE,
  find                         = SDL_SCANCODE_FIND,
  mute                         = SDL_SCANCODE_MUTE,
  volume_up                    = SDL_SCANCODE_VOLUMEUP,
  volume_down                  = SDL_SCANCODE_VOLUMEDOWN,
  keypad_comma                 = SDL_SCANCODE_KP_COMMA,
  keypad_equals_as_400         = SDL_SCANCODE_KP_EQUALSAS400,
  international_1              = SDL_SCANCODE_INTERNATIONAL1,
  international_2              = SDL_SCANCODE_INTERNATIONAL2,
  international_3              = SDL_SCANCODE_INTERNATIONAL3,
  international_4              = SDL_SCANCODE_INTERNATIONAL4,
  international_5              = SDL_SCANCODE_INTERNATIONAL5,
  international_6              = SDL_SCANCODE_INTERNATIONAL6,
  international_7              = SDL_SCANCODE_INTERNATIONAL7,
  international_8              = SDL_SCANCODE_INTERNATIONAL8,
  international_9              = SDL_SCANCODE_INTERNATIONAL9,
  language_1                   = SDL_SCANCODE_LANG1,
  language_2                   = SDL_SCANCODE_LANG2,
  language_3                   = SDL_SCANCODE_LANG3,
  language_4                   = SDL_SCANCODE_LANG4,
  language_5                   = SDL_SCANCODE_LANG5,
  language_6                   = SDL_SCANCODE_LANG6,
  language_7                   = SDL_SCANCODE_LANG7,
  language_8                   = SDL_SCANCODE_LANG8,
  language_9                   = SDL_SCANCODE_LANG9,
  alt_erase                    = SDL_SCANCODE_ALTERASE,
  sys_req                      = SDL_SCANCODE_SYSREQ,
  cancel                       = SDL_SCANCODE_CANCEL,
  clear                        = SDL_SCANCODE_CLEAR,
  prior                        = SDL_SCANCODE_PRIOR,
  return_2                     = SDL_SCANCODE_RETURN2,
  separator                    = SDL_SCANCODE_SEPARATOR,
  out                          = SDL_SCANCODE_OUT,
  oper                         = SDL_SCANCODE_OPER,
  clear_again                  = SDL_SCANCODE_CLEARAGAIN,
  cr_sel                       = SDL_SCANCODE_CRSEL,
  ex_sel                       = SDL_SCANCODE_EXSEL,
  keypad_00                    = SDL_SCANCODE_KP_00,
  keypad_000                   = SDL_SCANCODE_KP_000,
  thousands_separator          = SDL_SCANCODE_THOUSANDSSEPARATOR,
  decimal_separator            = SDL_SCANCODE_DECIMALSEPARATOR,
  currency_unit                = SDL_SCANCODE_CURRENCYUNIT,
  currency_sub_unit            = SDL_SCANCODE_CURRENCYSUBUNIT,
  keypad_left_parenthesis      = SDL_SCANCODE_KP_LEFTPAREN,
  keypad_right_parenthesis     = SDL_SCANCODE_KP_RIGHTPAREN,
  keypad_left_brace            = SDL_SCANCODE_KP_LEFTBRACE,
  keypad_right_brace           = SDL_SCANCODE_KP_RIGHTBRACE,
  keypad_tab                   = SDL_SCANCODE_KP_TAB,
  keypad_backspace             = SDL_SCANCODE_KP_BACKSPACE,
  keypad_a                     = SDL_SCANCODE_KP_A,
  keypad_b                     = SDL_SCANCODE_KP_B,
  keypad_c                     = SDL_SCANCODE_KP_C,
  keypad_d                     = SDL_SCANCODE_KP_D,
  keypad_e                     = SDL_SCANCODE_KP_E,
  keypad_f                     = SDL_SCANCODE_KP_F,
  keypad_xor                   = SDL_SCANCODE_KP_XOR,
  keypad_power                 = SDL_SCANCODE_KP_POWER,
  keypad_percent               = SDL_SCANCODE_KP_PERCENT,
  keypad_less                  = SDL_SCANCODE_KP_LESS,
  keypad_greater               = SDL_SCANCODE_KP_GREATER,
  keypad_ampersand             = SDL_SCANCODE_KP_AMPERSAND,
  keypad_double_ampersand      = SDL_SCANCODE_KP_DBLAMPERSAND,
  keypad_vertical_bar          = SDL_SCANCODE_KP_VERTICALBAR,
  keypad_double_vertical_bar   = SDL_SCANCODE_KP_DBLVERTICALBAR,
  keypad_colon                 = SDL_SCANCODE_KP_COLON,
  keypad_hash                  = SDL_SCANCODE_KP_HASH,
  keypad_space                 = SDL_SCANCODE_KP_SPACE,
  keypad_at                    = SDL_SCANCODE_KP_AT,
  keypad_exclamation           = SDL_SCANCODE_KP_EXCLAM,
  keypad_mem_store             = SDL_SCANCODE_KP_MEMSTORE,
  keypad_mem_recall            = SDL_SCANCODE_KP_MEMRECALL,
  keypad_mem_clear             = SDL_SCANCODE_KP_MEMCLEAR,
  keypad_mem_add               = SDL_SCANCODE_KP_MEMADD,
  keypad_mem_subtract          = SDL_SCANCODE_KP_MEMSUBTRACT,
  keypad_mem_multiply          = SDL_SCANCODE_KP_MEMMULTIPLY,
  keypad_mem_divide            = SDL_SCANCODE_KP_MEMDIVIDE,
  keypad_plus_minus            = SDL_SCANCODE_KP_PLUSMINUS,
  keypad_clear                 = SDL_SCANCODE_KP_CLEAR,
  keypad_clear_entry           = SDL_SCANCODE_KP_CLEARENTRY,
  keypad_binary                = SDL_SCANCODE_KP_BINARY,
  keypad_octal                 = SDL_SCANCODE_KP_OCTAL,
  keypad_decimal               = SDL_SCANCODE_KP_DECIMAL,
  keypad_hexadecimal           = SDL_SCANCODE_KP_HEXADECIMAL,
  left_ctrl                    = SDL_SCANCODE_LCTRL,
  left_shift                   = SDL_SCANCODE_LSHIFT,
  left_alt                     = SDL_SCANCODE_LALT,
  left_gui                     = SDL_SCANCODE_LGUI,
  right_ctrl                   = SDL_SCANCODE_RCTRL,
  right_shift                  = SDL_SCANCODE_RSHIFT,
  right_alt                    = SDL_SCANCODE_RALT,
  right_gui                    = SDL_SCANCODE_RGUI,
  mode                         = SDL_SCANCODE_MODE,
  audio_next                   = SDL_SCANCODE_AUDIONEXT,
  audio_prev                   = SDL_SCANCODE_AUDIOPREV,
  audio_stop                   = SDL_SCANCODE_AUDIOSTOP,
  audio_play                   = SDL_SCANCODE_AUDIOPLAY,
  audio_mute                   = SDL_SCANCODE_AUDIOMUTE,
  media_select                 = SDL_SCANCODE_MEDIASELECT,
  www                          = SDL_SCANCODE_WWW,
  mail                         = SDL_SCANCODE_MAIL,
  calculator                   = SDL_SCANCODE_CALCULATOR,
  computer                     = SDL_SCANCODE_COMPUTER,
  ac_search                    = SDL_SCANCODE_AC_SEARCH,
  ac_home                      = SDL_SCANCODE_AC_HOME,
  ac_back                      = SDL_SCANCODE_AC_BACK,
  ac_forward                   = SDL_SCANCODE_AC_FORWARD,
  ac_stop                      = SDL_SCANCODE_AC_STOP,
  ac_refresh                   = SDL_SCANCODE_AC_REFRESH,
  ac_bookmarks                 = SDL_SCANCODE_AC_BOOKMARKS,
  brightness_down              = SDL_SCANCODE_BRIGHTNESSDOWN,
  brightness_up                = SDL_SCANCODE_BRIGHTNESSUP,
  display_switch               = SDL_SCANCODE_DISPLAYSWITCH,
  keyboard_illumination_toggle = SDL_SCANCODE_KBDILLUMTOGGLE,
  keyboard_illumination_down   = SDL_SCANCODE_KBDILLUMDOWN,
  keyboard_illumination_up     = SDL_SCANCODE_KBDILLUMUP,
  eject                        = SDL_SCANCODE_EJECT,
  sleep                        = SDL_SCANCODE_SLEEP,
  app_1                        = SDL_SCANCODE_APP1,
  app_2                        = SDL_SCANCODE_APP2,
  audio_rewind                 = SDL_SCANCODE_AUDIOREWIND,
  audio_fast_forward           = SDL_SCANCODE_AUDIOFASTFORWARD,
  soft_left                    = SDL_SCANCODE_SOFTLEFT,
  soft_right                   = SDL_SCANCODE_SOFTRIGHT,
  call                         = SDL_SCANCODE_CALL,
  end_call                     = SDL_SCANCODE_ENDCALL//,
  //num_scan_codes             = SDL_NUM_SCANCODES
};

inline constexpr std::size_t num_scan_codes = SDL_NUM_SCANCODES;
}