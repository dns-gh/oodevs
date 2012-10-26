// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PROPAGATION_COMMAND_H
#define SWORD_PROPAGATION_COMMAND_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
    class Event;
}
namespace propagation
{
// =============================================================================
/** @class  PropagationCommand
    @brief  Propagation command
*/
// Created: LGY 2012-09-20
// =============================================================================
class PropagationCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    PropagationCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

public:
    //! @name Operations
    //@{
    void Destroy( const wrapper::View& parameters, const wrapper::View& model ) const;
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_PROPAGATION_COMMAND_H
