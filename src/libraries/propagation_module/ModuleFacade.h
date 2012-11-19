// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef movement_module_ModuleFacade_h
#define movement_module_ModuleFacade_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <wrapper/Module.h>

namespace sword
{
namespace propagation
{
// =============================================================================
/** @class  ModuleFacade
    @brief  Module facade
*/
// Created: LGY 2012-06-12
// =============================================================================
class ModuleFacade : private sword::wrapper::Module
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModuleFacade( const wrapper::View& model );
    //@}
};

}
}

#endif // movement_module_ModuleFacade_h
