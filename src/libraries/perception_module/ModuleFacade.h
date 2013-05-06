// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef perception_module_ModuleFacade_h
#define perception_module_ModuleFacade_h

#include <wrapper/Module.h>
#include <boost/noncopyable.hpp>

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  ModuleFacade
    @brief  Module facade
*/
// Created: SLI 2012-03-16
// =============================================================================
class ModuleFacade : private sword::wrapper::Module
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModuleFacade( const wrapper::View& model );
            ~ModuleFacade();
    //@}
};

}
}

#endif // perception_module_ModuleFacade_h