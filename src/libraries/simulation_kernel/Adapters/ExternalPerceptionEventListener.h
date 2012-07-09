// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H
#define SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H

#include "tools/Resolver.h"
#include <core/EventListener_ABC.h>

class MIL_AgentPion;

namespace core
{
    class Facade;
}

namespace sword
{
// =============================================================================
/** @class  ExternalPerceptionEventListener
    @brief  Direct fire pion attack event listener
*/
// Created: MCO 2012-01-26
// =============================================================================
class ExternalPerceptionEventListener : public core::EventListener_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             ExternalPerceptionEventListener( const core::Model& model, core::Facade& facade, tools::Resolver< MIL_AgentPion >& resolver );
    virtual ~ExternalPerceptionEventListener();
    //@}

    //! @name Operations
    //@{
    virtual void Notify( const core::Model& event );
    //@}

private:
    //! @name Member data
    //@{
    const core::Model& model_;
    core::Facade& facade_;
    tools::Resolver< MIL_AgentPion >& resolver_;
    //@}
};

}

#endif // SWORD_EXTERNAL_PERCEPTION_EVENT_LISTENER_H
