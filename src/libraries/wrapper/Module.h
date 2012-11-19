// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_MODULE_H
#define WRAPPER_MODULE_H

#include "Hook.h"
#include "Command.h"
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;

// =============================================================================
/** @class  Node
    @brief  Node
*/
// Created: SLI 2011-12-07
// =============================================================================
class Module : private Hooks, private detail::Commands, private boost::noncopyable
{
protected:
    //! @name Constructors/Destructor
    //@{
    Module( const View& model )
        : Hooks   ( model )
        , Commands( model )
    {
        // NOTHING
    }
    //@}
};

}
}

#endif // WRAPPER_MODULE_H
