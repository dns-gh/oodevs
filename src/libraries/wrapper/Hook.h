// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_HOOK_H
#define WRAPPER_HOOK_H

#include <module_api/Hook.h>
#include <functional>
#include <algorithm>
#include <vector>

namespace sword
{
namespace wrapper
{
    class Hook_ABC
    {
    public:
        virtual void Apply() = 0;
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
    protected:
        Hooks()
        {
            std::for_each( GetUses().begin(), GetUses().end(), std::mem_fun( &Hook_ABC::Apply ) );
            std::for_each( GetRegistrations().begin(), GetRegistrations().end(), std::mem_fun( &Hook_ABC::Apply ) );
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
}

#define DECLARE_HOOK( Hook, result, parameters ) \
    static struct Hook ## Wrapper : private sword::wrapper::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            sword::wrapper::Hooks::Use( this ); \
        } \
        typedef result (*Function) parameters; \
        operator Function const() \
        { \
            return current_; \
        } \
        Function current_; \
    private: \
        virtual void Apply() \
        { \
            SWORD_UseHook( reinterpret_cast< SWORD_Hook* >( &current_ ), \
                           #Hook, #result #parameters ); \
        } \
    } Hook;

#define DEFINE_HOOK( Hook, result, parameters ) \
    static struct Hook ## Wrapper : private sword::wrapper::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            sword::wrapper::Hooks::Register( this ); \
        } \
        typedef result (*Function) parameters; \
        Function current_; \
        Function Previous; \
        static result Implement parameters; \
    private: \
        virtual void Apply() \
        { \
            SWORD_RegisterHook( reinterpret_cast< SWORD_Hook* >( &current_ ), \
                                reinterpret_cast< SWORD_Hook* >( &Previous ), \
                                reinterpret_cast< SWORD_Hook >( &Implement ), \
                                #Hook, #result #parameters ); \
        } \
    } Hook; \
    result Hook ## Wrapper::Implement parameters

// $$$$ MCO : or Hook.current_ if we want to keep the macro
#define GET_HOOK( Hook ) Hook

// $$$$ MCO : simply Previous should be possible, Previous has to be static in the wrapper though
#define GET_PREVIOUS_HOOK( Hook ) Hook.Previous

#endif // WRAPPER_HOOK_H
