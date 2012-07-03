// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_ModuleFacade_h
#define fire_module_ModuleFacade_h

#include <wrapper/Hook.h>
#include <boost/noncopyable.hpp>

namespace sword
{
namespace fire
{
// =============================================================================
/** @class  ModuleFacade
    @brief  Module facade
*/
// Created: MCO 2012-03-19
// =============================================================================
class ModuleFacade : private sword::wrapper::Hooks, private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    ModuleFacade();
    //@}
};

}
}

#endif // fire_module_ModuleFacade_h
