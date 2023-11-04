#pragma once

#include <SDL_blendmode.h>

namespace sdl
{
enum class blend_mode
{
  none                = SDL_BLENDMODE_NONE,
  blend               = SDL_BLENDMODE_BLEND,
  add                 = SDL_BLENDMODE_ADD,
  mod                 = SDL_BLENDMODE_MOD,
  mul                 = SDL_BLENDMODE_MUL,
  invalid             = SDL_BLENDMODE_INVALID
};
enum class blend_operation
{
  add                 = SDL_BLENDOPERATION_ADD,
  subtract            = SDL_BLENDOPERATION_SUBTRACT,
  rev_subtract        = SDL_BLENDOPERATION_REV_SUBTRACT,
  minimum             = SDL_BLENDOPERATION_MINIMUM,
  maximum             = SDL_BLENDOPERATION_MAXIMUM
};
enum class blend_factor
{
  zero                = SDL_BLENDFACTOR_ZERO,
  one                 = SDL_BLENDFACTOR_ONE,
  src_color           = SDL_BLENDFACTOR_SRC_COLOR,
  one_minus_src_color = SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR,
  src_alpha           = SDL_BLENDFACTOR_SRC_ALPHA,
  one_minus_src_alpha = SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
  dst_color           = SDL_BLENDFACTOR_DST_COLOR,
  one_minus_dst_color = SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,
  dst_alpha           = SDL_BLENDFACTOR_DST_ALPHA,
  one_minus_dst_alpha = SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA
};

inline blend_mode compose_custom_blend_mode(
  blend_factor    src_color_factor, 
  blend_factor    dst_color_factor, 
  blend_operation color_operation , 
  blend_factor    src_alpha_factor, 
  blend_factor    dst_alpha_factor, 
  blend_operation alpha_operation )
{
  return static_cast<blend_mode>(SDL_ComposeCustomBlendMode(
    static_cast<SDL_BlendFactor>   (src_color_factor), 
    static_cast<SDL_BlendFactor>   (dst_color_factor), 
    static_cast<SDL_BlendOperation>(color_operation ), 
    static_cast<SDL_BlendFactor>   (src_alpha_factor), 
    static_cast<SDL_BlendFactor>   (dst_alpha_factor), 
    static_cast<SDL_BlendOperation>(alpha_operation )));
}
}