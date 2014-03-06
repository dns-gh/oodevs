// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef DISPATCHER_CHECKPOINTFILTERPLUGIN_H
#define DISPATCHER_CHECKPOINTFILTERPLUGIN_H

#include "PluginContainer.h"

namespace dispatcher
{
class LinkResolver_ABC;
class ClientPublisher_ABC;

class CheckpointFilterPlugin: public PluginContainer
{
public:
    explicit CheckpointFilterPlugin( const LinkResolver_ABC& resolver );
    virtual ~CheckpointFilterPlugin();

    virtual void NotifySimulationLeft();

private:
    virtual bool ForwardSimToClient( const sword::SimToClient& message );
    void Reset();

private:
    const LinkResolver_ABC& resolver_;
    bool checkpointInProgress_;
    ClientPublisher_ABC* client_;
};

}  // namespace dispatcher

#endif //DISPATCHER_CHECKPOINTFILTERPLUGIN_H
