#pragma once

#include <cstdint>
#include <limits>
#include <optional>
#include <span>

#include <SDL_rect.h>

namespace sdl
{
using native_point  = SDL_Point ;
using native_fpoint = SDL_FPoint;

using native_rect   = SDL_Rect  ;
using native_frect  = SDL_FRect ;

struct line
{
  native_point  start;
  native_point  end  ;
};
struct fline
{
  native_fpoint start;
  native_fpoint end  ;
};

[[nodiscard]]
inline bool                        rect_empty              (const native_rect&  rectangle)
{
  return static_cast<bool>(SDL_RectEmpty            (&rectangle));
}
[[nodiscard]]
inline bool                        rect_equals             (const native_rect&  lhs, const native_rect&  rhs)
{
  return static_cast<bool>(SDL_RectEquals           (&lhs, &rhs));
}
[[nodiscard]]
inline bool                        has_intersection        (const native_rect&  lhs, const native_rect&  rhs)
{
  return static_cast<bool>(SDL_HasIntersection      (&lhs, &rhs));
}
[[nodiscard]]
inline std::optional<native_rect>  intersect_rect          (const native_rect&  lhs, const native_rect&  rhs)
{
  native_rect result;
  if (SDL_IntersectRect(&lhs, &rhs, &result) == SDL_FALSE)
    return std::nullopt;
  return result;
}
[[nodiscard]]
inline native_rect                 union_rect              (const native_rect&  lhs, const native_rect&  rhs)
{
  native_rect result;
  SDL_UnionRect(&lhs, &rhs, &result);
  return result;
}
[[nodiscard]]
inline bool                        point_in_rect           (const native_point&             point    , const native_rect&                 rectangle)
{
  return static_cast<bool>(SDL_PointInRect          (&point, &rectangle));
}
[[nodiscard]]
inline std::optional<native_rect>  enclose_points          (const std::span<native_point>&  points   , const std::optional<native_rect>&  clip = std::nullopt)
{
  native_rect result;
  if (SDL_EnclosePoints(points.data(), static_cast<std::int32_t>(points.size()), clip ? &clip.value() : nullptr, &result) == SDL_FALSE)
    return std::nullopt;
  return result;
}
[[nodiscard]]
inline bool                        intersect_rect_and_line (const native_rect&              rectangle, line&                              line)
{
  return static_cast<bool>(SDL_IntersectRectAndLine (&rectangle, &line.start.x, &line.start.y, &line.end.x, &line.end.y));
}

[[nodiscard]]
inline bool                        frect_empty             (const native_frect& rectangle)
{
  return static_cast<bool>(SDL_FRectEmpty           (&rectangle));
}
[[nodiscard]]
inline bool                        frect_equals            (const native_frect& lhs, const native_frect& rhs)
{
  return static_cast<bool>(SDL_FRectEquals          (&lhs, &rhs));
}
[[nodiscard]]
inline bool                        frect_equals_epsilon    (const native_frect& lhs, const native_frect& rhs, const float epsilon = std::numeric_limits<float>::epsilon())
{
  return static_cast<bool>(SDL_FRectEqualsEpsilon   (&lhs, &rhs, epsilon));
}
[[nodiscard]]
inline bool                        has_intersectionf       (const native_frect& lhs, const native_frect& rhs)
{
  return static_cast<bool>(SDL_HasIntersectionF     (&lhs, &rhs));
}
[[nodiscard]]
inline std::optional<native_frect> intersect_frect         (const native_frect& lhs, const native_frect& rhs)
{
  native_frect result;
  if (SDL_IntersectFRect(&lhs, &rhs, &result) == SDL_FALSE)
    return std::nullopt;
  return result;
}
[[nodiscard]]
inline native_frect                union_frect             (const native_frect& lhs, const native_frect& rhs)
{
  native_frect result;
  SDL_UnionFRect(&lhs, &rhs, &result);
  return result;
}
[[nodiscard]]
inline bool                        point_in_frect          (const native_fpoint&            point    , const native_frect&                rectangle)
{
  return static_cast<bool>(SDL_PointInFRect         (&point, &rectangle));
}
[[nodiscard]]
inline std::optional<native_frect> enclose_fpoints         (const std::span<native_fpoint>& points   , const std::optional<native_frect>& clip = std::nullopt)
{
  native_frect result;
  if (SDL_EncloseFPoints(points.data(), static_cast<std::int32_t>(points.size()), clip ? &clip.value() : nullptr, &result) == SDL_FALSE)
    return std::nullopt;
  return result;
}
[[nodiscard]]
inline bool                        intersect_frect_and_line(const native_frect&             rectangle, fline&                             line)
{
  return static_cast<bool>(SDL_IntersectFRectAndLine(&rectangle, &line.start.x, &line.start.y, &line.end.x, &line.end.y));
}

// Conveniences.

template <typename type>
struct rectangle;

template <>
struct rectangle<std::int32_t> : native_rect
{
  rectangle            (const native_rect& native)
  : native_rect {native}
  {

  }
  rectangle            (const std::int32_t position_x, const std::int32_t position_y, const std::int32_t width, const std::int32_t height)
  : native_rect {position_x, position_y, width, height}
  {

  }
  explicit rectangle   (const std::span<native_point>& points, const std::optional<rectangle>& clip = std::nullopt)
  : native_rect {enclose_points(points, clip ? clip.value() : std::optional<native_rect>()).value_or(native_rect{})}
  {

  }
  rectangle            ()                       = default;
  rectangle            (const rectangle&  that) = default;
  rectangle            (      rectangle&& temp) = default;
 ~rectangle            ()                       = default;
  rectangle& operator= (const rectangle&  that) = default;
  rectangle& operator= (      rectangle&& temp) = default;
  bool       operator==(const rectangle&  that) const
  {
    return  rect_equals(*this, that);
  }
  bool       operator!=(const rectangle&  that) const
  {
    return !rect_equals(*this, that);
  }

  [[nodiscard]]
  bool                     empty         () const
  {
    return rect_empty             (*this);
  }
  [[nodiscard]]
  bool                     intersects    (const rectangle&     that ) const
  {
    return has_intersection       (*this, that);
  }
  [[nodiscard]]
  std::optional<rectangle> intersect     (const rectangle&     that ) const
  {
    auto result = intersect_rect(*this, that);
    if (!result)
      return std::nullopt;
    return result.value();
  }
  [[nodiscard]]
  rectangle                merged        (const rectangle&     that ) const
  {
    return union_rect             (*this, that);
  }
  [[nodiscard]]
  bool                     contains      (const native_point&  point) const
  {
    return point_in_rect          (point, *this);
  }
  [[nodiscard]]
  bool                     clip_line     (line&                line ) const
  {
    return intersect_rect_and_line(*this, line);
  }
};

template <>
struct rectangle<float>        : native_frect
{
  rectangle            (const native_frect& native)
  : native_frect {native}
  {

  }
  rectangle            (const float position_x, const float position_y, const float width, const float height)
  : native_frect {position_x, position_y, width, height}
  {

  }
  explicit rectangle   (const std::span<native_fpoint>& points, const std::optional<rectangle>& clip = std::nullopt)
  : native_frect {enclose_fpoints(points, clip ? clip.value() : std::optional<native_frect>()).value_or(native_frect{})}
  {

  }
  rectangle            ()                       = default;
  rectangle            (const rectangle&  that) = default;
  rectangle            (      rectangle&& temp) = default;
 ~rectangle            ()                       = default;
  rectangle& operator= (const rectangle&  that) = default;
  rectangle& operator= (      rectangle&& temp) = default;
  bool       operator==(const rectangle&  that) const
  {
    return  frect_equals(*this, that);
  }
  bool       operator!=(const rectangle&  that) const
  {
    return !frect_equals(*this, that);
  }

  [[nodiscard]]
  bool                     empty         () const
  {
    return frect_empty             (*this);
  }
  [[nodiscard]]
  bool                     equals_epsilon(const native_frect&  that , const float epsilon = std::numeric_limits<float>::epsilon()) const
  {
    return frect_equals_epsilon(*this, that, epsilon);
  }
  [[nodiscard]]
  bool                     intersects    (const rectangle&     that ) const
  {
    return has_intersectionf       (*this, that);
  }
  [[nodiscard]]
  std::optional<rectangle> intersect     (const rectangle&     that ) const
  {
    auto result = intersect_frect(*this, that);
    if (!result)
      return std::nullopt;
    return result.value();
  }
  [[nodiscard]]
  rectangle                merged        (const rectangle&     that ) const
  {
    return union_frect             (*this, that);
  }
  [[nodiscard]]
  bool                     contains      (const native_fpoint& point) const
  {
    return point_in_frect          (point, *this);
  }
  [[nodiscard]]
  bool                     clip_line     (fline&               line ) const
  {
    return intersect_frect_and_line(*this, line);
  }
};
}