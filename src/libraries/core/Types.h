// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_API_TYPES_H
#define SWORD_API_TYPES_H

#if defined(_MSC_VER) && _MSC_VER < 1600
    typedef signed   __int8  int8_t;
    typedef unsigned __int8  uint8_t;
    typedef signed   __int16 int16_t;
    typedef unsigned __int16 uint16_t;
    typedef signed   __int32 int32_t;
    typedef unsigned __int32 uint32_t;
    typedef signed   __int64 int64_t;
    typedef unsigned __int64 uint64_t;
    #include <climits>
#else
#   ifdef BOOST__STDC_CONSTANT_MACROS_DEFINED
#       undef INT8_C
#       undef INT16_C
#       undef INT32_C
#       undef INT64_C
#       undef UINT8_C
#       undef UINT16_C
#       undef UINT32_C
#       undef UINT64_C
#       undef INTMAX_C
#       undef UINTMAX_C
#   endif
#   define __STDC_LIMIT_MACROS
#   include <stdint.h>
#   define BOOST__STDC_CONSTANT_MACROS_DEFINED
#endif

#endif // SWORD_API_TYPES_H
