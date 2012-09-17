// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PERCEPTION_MODULE_PERCEPTION_COMMAND_H
#define PERCEPTION_MODULE_PERCEPTION_COMMAND_H

#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace perception
{
    class ModuleFacade;

// =============================================================================
/** @class  PerceptionCommand
    @brief  Perception command
*/
// Created: SLI 2012-03-16
// =============================================================================
class PerceptionCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    PerceptionCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // PERCEPTION_MODULE_PERCEPTION_COMMAND_H
