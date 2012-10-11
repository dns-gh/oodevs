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
#include <wrapper/View.h>
#include <boost/noncopyable.hpp>
#include <map>

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

    //! @name Member data
    //@{
    std::map< const SWORD_Model*, std::pair< double, std::size_t > > weapons;
    //@}
};

}
}

#endif // fire_module_ModuleFacade_h
