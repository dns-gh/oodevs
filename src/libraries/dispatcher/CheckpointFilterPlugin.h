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

namespace plugins
{
namespace rights
{
    class RightsPlugin;
}
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace dispatcher
{
class CheckpointFilterPlugin: public PluginContainer
{
public:
    explicit CheckpointFilterPlugin( const plugins::rights::RightsPlugin& rights );
    virtual ~CheckpointFilterPlugin();

    virtual void NotifySimulationLeft();

private:
    virtual bool ForwardSimToClient( const sword::SimToClient& message );
    void Reset();

private:
    const plugins::rights::RightsPlugin& rights_;
    bool checkpointInProgress_;
    ClientPublisher_ABC* client_;
};

}  // namespace dispatcher

#endif //DISPATCHER_CHECKPOINTFILTERPLUGIN_H
