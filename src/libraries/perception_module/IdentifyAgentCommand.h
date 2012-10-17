// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_IDENTIFY_AGENT_COMMAND_H
#define SWORD_PERCEPTION_IDENTIFY_AGENT_COMMAND_H

#include "wrapper/View.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
namespace wrapper
{
    class View;
}

namespace perception
{
// =============================================================================
/** @class  IdentifyAgentCommand
    @brief  Identify agent command
*/
// Created: SLI 2012-03-29
// =============================================================================
class IdentifyAgentCommand : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    IdentifyAgentCommand( const wrapper::View& parameters, const wrapper::View& model );
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_IDENTIFY_AGENT_COMMAND_H
