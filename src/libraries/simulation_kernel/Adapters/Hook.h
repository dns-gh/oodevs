// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_HOOK_TOOLS_H
#define SWORD_HOOK_TOOLS_H

#include <core/Facade.h>

#define DECLARE_HOOK( Hook, result, parameters ) \
    namespace \
    { \
        typedef result (*HOOK_ ## Hook) parameters; \
        __pragma(warning(suppress:4100)) \
        result HOOK_ ## Hook ## _Unitialized parameters \
        { \
            throw std::runtime_error( "Missing USE_HOOK for " #Hook ); \
        } \
        HOOK_ ## Hook HOOK_ ## Hook ## _Current = &HOOK_ ## Hook ## _Unitialized; \
        HOOK_ ## Hook HOOK_ ## Hook ## _Previous = &HOOK_ ## Hook ## _Unitialized; \
        void HOOK_Register ## Hook( const HOOK_ ## Hook implement, core::Facade& facade ) \
        { \
            facade.RegisterHook( &HOOK_ ## Hook ## _Current, &HOOK_ ## Hook ## _Previous, implement, #Hook, #result #parameters ); \
        } \
        void HOOK_Use ## Hook( core::Facade& facade ) \
        { \
            facade.UseHook( &HOOK_ ## Hook ## _Current, #Hook, #result #parameters ); \
        } \
    }

#define DEFINE_HOOK( Hook, result, parameters ) \
    DECLARE_HOOK( Hook, result, parameters ) \
    result HOOK_ ## Hook ## _Implement ## parameters

#define REGISTER_HOOK( Hook, facade ) \
    HOOK_Register ## Hook( HOOK_ ## Hook ## _Implement, facade )

#define USE_HOOK( Hook, facade ) \
    HOOK_Use ## Hook( facade )

#define GET_HOOK( Hook ) HOOK_ ## Hook ## _Current

#define GET_PREVIOUS_HOOK( Hook ) HOOK_ ## Hook ## _Previous

#endif // SWORD_HOOK_TOOLS_H
