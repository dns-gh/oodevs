// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef movement_module_ComputeHeightCommand_h
#define movement_module_ComputeHeightCommand_h

#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace movement
{
// =============================================================================
/** @class  ComputeHeightCommand
    @brief  Compute height command
*/
// Created: MCO 2012-03-12
// =============================================================================
class ComputeHeightCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    ComputeHeightCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // movement_module_ComputeHeightCommand_h
