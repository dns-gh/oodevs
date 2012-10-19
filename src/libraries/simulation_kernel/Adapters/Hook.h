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
#include <algorithm>
#include <vector>
#include <boost/bind.hpp>

namespace sword
{
    class Hook_ABC
    {
    public:
        virtual void Apply( core::Facade& facade ) = 0;
    protected:
        virtual ~Hook_ABC()
        {}
    };

    class Hooks
    {
    public:
        static void Use( Hook_ABC* hook )
        {
            GetUses().push_back( hook );
        }
        static void Register( Hook_ABC* hook )
        {
            GetRegistrations().push_back( hook );
        }
        static void Initialize( core::Facade& facade )
        {
            std::for_each( GetUses().begin(), GetUses().end(), boost::bind( &Hook_ABC::Apply, _1, boost::ref( facade ) ) );
            std::for_each( GetRegistrations().begin(), GetRegistrations().end(), boost::bind( &Hook_ABC::Apply, _1, boost::ref( facade ) ) );
        }
    private:
        static std::vector< Hook_ABC* >& GetUses()
        {
            static std::vector< Hook_ABC* > hooks;
            return hooks;
        }
        static std::vector< Hook_ABC* >& GetRegistrations()
        {
            static std::vector< Hook_ABC* > hooks;
            return hooks;
        }
    };
}

#define DECLARE_HOOK( Hook, result, parameters ) \
    static struct Hook ## Wrapper : private sword::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            sword::Hooks::Use( this ); \
        } \
        typedef result (*Function) parameters; \
        operator Function const() \
        { \
            return current_; \
        } \
        Function current_; \
    private: \
        virtual void Apply( core::Facade& facade ) \
        { \
            facade.UseHook( &current_, #Hook, #result #parameters ); \
        } \
} Hook##_;

#define DEFINE_HOOK( Hook, result, parameters ) \
    static struct Hook ## Wrapper : private sword::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            sword::Hooks::Register( this ); \
        } \
        typedef result (*Function) parameters; \
        operator Function const() \
        { \
            return current_; \
        } \
        Function current_; \
        Function Previous; \
        static result Implement parameters; \
    private: \
        virtual void Apply( core::Facade& facade ) \
        { \
            facade.RegisterHook( &current_, &Previous, Implement, #Hook, #result #parameters ); \
        } \
    } Hook##_; \
    result Hook ## Wrapper::Implement parameters

// $$$$ MCO : or Hook.current_ if we want to keep the macro
#define GET_HOOK( Hook ) Hook##_

// $$$$ MCO : simply Previous should be possible, Previous has to be static in the wrapper though
#define GET_PREVIOUS_HOOK( Hook ) Hook##_.Previous

#endif // SWORD_HOOK_TOOLS_H
