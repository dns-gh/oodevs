// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_plugin_ResourceNetworkUpdater_h_
#define __crossbow_plugin_ResourceNetworkUpdater_h_

namespace sword
{
    class ResourceNetwork;
    class ResourceNetwork_Link;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Table_ABC;

// =============================================================================
/** @class  ResourceNetworkUpdater
    @brief  ResourceNetworkUpdater
*/
// Created: AME 2011-01-26
// =============================================================================
class ResourceNetworkUpdater
{

public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkUpdater( Workspace_ABC& workspace, long oid, int session_id, bool creation );
    virtual ~ResourceNetworkUpdater();
    //@}

    //! @name Operations
    //@{
     void Update( const sword::ResourceNetwork& network );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourceNetworkUpdater( const ResourceNetworkUpdater& );            //!< Copy constructor
    ResourceNetworkUpdater& operator=( const ResourceNetworkUpdater& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void UpdateResourceNetworkLinks( const sword::ResourceNetwork& network, long network_id );
    void UpdateResourceNetworkLink( const sword::ResourceNetwork_Link& link, long network_id );
    //@}

private:
    //! @name Member data
    //@{
    Workspace_ABC& workspace_;
    long objectId_;
    int sessionId_;
    bool creation_;
    //@}
};
}
}
#endif // __crossbow_plugin_ResourceNetworkUpdater_h_
