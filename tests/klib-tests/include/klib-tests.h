#ifndef __KLIB_TESTS_H__
#define __KLIB_TESTS_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define CASE(id, entry_, ...) \
  case id: { \
    void entry_(); \
    entry = entry_; \
    __VA_ARGS__; \
    entry_(); \
    break; \
  }
#endif