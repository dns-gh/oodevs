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
    class ModuleFacade;

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
             IdentifyAgentCommand( ModuleFacade& module, const wrapper::View& parameters, const wrapper::View& model, size_t identifier );
    virtual ~IdentifyAgentCommand();
    //@}

    //! @name Operations
    //@{
    void Execute( const wrapper::View& parameters, const wrapper::View& model ) const;
    void ExecutePaused( const wrapper::View& parameters, const wrapper::View& model ) const;
    //@}

private:
    //! @name Member data
    //@{
    const size_t identifier_;
    const wrapper::View localization_;
    //@}

};

}
}

#endif // SWORD_PERCEPTION_IDENTIFY_AGENT_COMMAND_H
