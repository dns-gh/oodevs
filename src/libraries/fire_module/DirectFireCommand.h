// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_DirectFireCommand_h
#define fire_module_DirectFireCommand_h

#include "RoleAction_DirectFiring.h"
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
    class ModuleFacade;

// =============================================================================
/** @class  DirectFireCommand
    @brief  Direct fire command
*/
// Created: MCO 2012-03-19
// =============================================================================
class DirectFireCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
     DirectFireCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, std::size_t identifier );
    ~DirectFireCommand();
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void PostCallback( int code ) const;
    //@}

private:
    //! @name Member data
    //@{
    const size_t action_;
    const unsigned int identifier_;
    const RoleAction_DirectFiring role_;
    mutable bool mustReport_;
    //@}
};

}
}

#endif // fire_module_DirectFireCommand_h
