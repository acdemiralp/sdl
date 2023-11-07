#pragma once

#include <cstdint>

#include <SDL_atomic.h>

namespace sdl
{
[[nodiscard]]
inline bool         atomic_try_lock                (std::int32_t* spin_lock)
{
  return static_cast<bool>(SDL_AtomicTryLock(spin_lock));
}
inline void         atomic_lock                    (std::int32_t* spin_lock)
{
  SDL_AtomicLock  (spin_lock);
}
inline void         atomic_unlock                  (std::int32_t* spin_lock)
{
  SDL_AtomicUnlock(spin_lock);
}

inline void         compiler_barrier               ()
{
  SDL_CompilerBarrier();
}
inline void         memory_barrier_acquire_function()
{
  SDL_MemoryBarrierAcquireFunction();
}
inline void         memory_barrier_release_function()
{
  SDL_MemoryBarrierReleaseFunction();
}
inline void         memory_barrier_acquire         ()
{
  SDL_MemoryBarrierAcquire();
}
inline void         memory_barrier_release         ()
{
  SDL_MemoryBarrierRelease();
}
//inline void       cpu_pause_instruction          ()
//{
//  SDL_CPUPauseInstruction();
//}

// Bad practice: You should use `std::atomic_int` instead.
using native_atomic_int = SDL_atomic_t;

// Bad practice: You should use `std::atomic_int` instead.
inline bool         atomic_cas                     (native_atomic_int* atomic, const std::int32_t old_value, const std::int32_t value)
{
  return static_cast<bool>(SDL_AtomicCAS(atomic, old_value, value));
}
// Bad practice: You should use `std::atomic_int` instead.
inline std::int32_t atomic_set                     (native_atomic_int* atomic, const std::int32_t value)
{
  return SDL_AtomicSet(atomic, value);
}
// Bad practice: You should use `std::atomic_int` instead.
[[nodiscard]]
inline std::int32_t atomic_get                     (native_atomic_int* atomic)
{
  return SDL_AtomicGet(atomic);
}
// Bad practice: You should use `std::atomic_int` instead.
inline std::int32_t atomic_add                     (native_atomic_int* atomic, const std::int32_t value)
{
  return SDL_AtomicAdd(atomic, value);
}
// Bad practice: You should use `std::atomic_int` instead.
inline std::int32_t atomic_inc_ref                 (native_atomic_int* atomic)
{
  return SDL_AtomicIncRef(atomic);
}
// Bad practice: You should use `std::atomic_int` instead.
inline bool         atomic_dec_ref                 (native_atomic_int* atomic)
{
  return SDL_AtomicDecRef(atomic);
}

// Bad practice: You should use `std::atomic<type*>` instead.
inline bool         atomic_cas_ptr                 (void** atomic, void* old_value, void* value)
{
  return static_cast<bool>(SDL_AtomicCASPtr(atomic, old_value, value));
}
// Bad practice: You should use `std::atomic<type*>` instead.
inline void*        atomic_set_ptr                 (void** atomic, void* value)
{
  return SDL_AtomicSetPtr(atomic, value);
}
// Bad practice: You should use `std::atomic<type*>` instead.
[[nodiscard]]
inline void*        atomic_get_ptr                 (void** atomic)
{
  return SDL_AtomicGetPtr(atomic);
}

// Conveniences.

class spin_lock
{
public:
  spin_lock           ()                       = default;
  spin_lock           (const spin_lock&  that) = delete ;
  spin_lock           (      spin_lock&& temp) = delete ;
 ~spin_lock           ()
  {
    if (is_locked())
      unlock();
  }
  spin_lock& operator=(const spin_lock&  that) = delete ;
  spin_lock& operator=(      spin_lock&& temp) = delete ;

  void         lock     ()
  {
    atomic_lock(&native_);
  }
  bool         try_lock ()
  {
    return atomic_try_lock(&native_);
  }
  void         unlock   ()
  {
    atomic_unlock(&native_);
  }

  [[nodiscard]]
  bool         is_locked() const
  {
    return native_ != 0;
  }
  [[nodiscard]]
  std::int32_t native   () const
  {
    return native_;
  }

private:
  std::int32_t native_ {};
};

// Bad practice: You should use `std::atomic_int` instead.
class atomic_int
{
public:
  atomic_int             () noexcept                  = default;
  explicit atomic_int    (const std::int32_t desired)
  : native_({desired})
  {
    
  }
  atomic_int             (const atomic_int&  that)    = delete ;
  atomic_int             (      atomic_int&& temp)    = delete ;
 ~atomic_int             ()                           = default;
  atomic_int&  operator= (const atomic_int&  that)    = delete ;
  atomic_int&  operator= (      atomic_int&& temp)    = delete ;
  std::int32_t operator= (const std::int32_t desired)
  {
    store (desired);
    return desired;
  }
  operator std::int32_t  ()
  {
    return load();
  }

  std::int32_t operator+=(const std::int32_t value)
  {
    return fetch_add(value) + value;
  }
  std::int32_t operator-=(const std::int32_t value)
  {
    return fetch_sub(value) - value;
  }
  std::int32_t operator++()
  {
    return fetch_add(1) + 1;
  }
  std::int32_t operator++(std::int32_t)
  {
    return fetch_add(1);
  }
  std::int32_t operator--()
  {
    return fetch_sub(1) - 1;
  }
  std::int32_t operator--(std::int32_t)
  {
    return fetch_sub(1);
  }

  [[nodiscard]]
  std::int32_t             load            ()
  {
    return atomic_get(&native_);
  }
  void                     store           (                             const std::int32_t desired)
  {
    atomic_set(&native_, desired);
  }
  std::int32_t             exchange        (                             const std::int32_t desired)
  {
    return atomic_set(&native_, desired);
  }
  bool                     compare_exchange(const std::int32_t expected, const std::int32_t desired)
  {
    return atomic_cas(&native_, expected, desired);
  }

  std::int32_t             fetch_add       (const std::int32_t value)
  {
    return atomic_add(&native_,  value);
  }
  std::int32_t             fetch_sub       (const std::int32_t value)
  {
    return atomic_add(&native_, -value);
  }

  [[nodiscard]]
  const native_atomic_int& native          () const
  {
    return native_;
  }

private:
  native_atomic_int native_ {};
};

// Bad practice: You should use `std::atomic<type*>` instead.
template <typename type = void>
class atomic_ptr
{
public:
  atomic_ptr            ()                           = default;
  explicit atomic_ptr   (type*              desired)
  : native_(desired)
  {
    
  }
  atomic_ptr            (const atomic_ptr&  that)    = delete ;
  atomic_ptr            (      atomic_ptr&& temp)    = delete ;
 ~atomic_ptr            ()                           = default;
  atomic_ptr& operator= (const atomic_ptr&  that)    = delete ;
  atomic_ptr& operator= (      atomic_ptr&& temp)    = delete ;
  type*       operator= (type*              desired)
  {
    store (desired);
    return desired;
  }
  operator type*        ()
  {
    return load();
  }

  [[nodiscard]]
  type*       load            ()
  {
    return static_cast<type*>(atomic_get_ptr(reinterpret_cast<void**>(&native_)));
  }
  void        store           (                type* desired)
  {
    atomic_set_ptr(reinterpret_cast<void**>(&native_), static_cast<void*>(desired));
  }
  type*       exchange        (                type* desired)
  {
    return static_cast<type*>(atomic_set_ptr(reinterpret_cast<void**>(&native_), static_cast<void*>(desired)));
  }
  bool        compare_exchange(type* expected, type* desired)
  {
    return atomic_cas_ptr(reinterpret_cast<void**>(&native_), static_cast<void*>(expected), static_cast<void*>(desired));
  }

  [[nodiscard]]
  const type* native          () const
  {
    return native_;
  }

private:
  type* native_ {};
};
}