// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef movement_module_OrientateCommand_h
#define movement_module_OrientateCommand_h

#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace movement
{
    class ModuleFacade;

// =============================================================================
/** @class  OrientateCommand
    @brief  Orientate command
*/
// Created: MCO 2012-03-09
// =============================================================================
class OrientateCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    OrientateCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // movement_module_OrientateCommand_h
