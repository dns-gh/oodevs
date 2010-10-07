// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_DatabaseUpdater_ABC_h_
#define __crossbow_DatabaseUpdater_ABC_h_

namespace MsgsSimToClient
{
    class MsgFormationCreation;
    class MsgAutomatCreation;
    class MsgAutomatAttributes;
    class MsgUnitCreation;
    class MsgUnitAttributes;
    class MsgUnitDestruction;
    class MsgUnitKnowledgeCreation;
    class MsgUnitKnowledgeUpdate;
    class MsgUnitKnowledgeDestruction;
    class MsgObjectCreation;
    class MsgObjectDestruction;
    class MsgObjectUpdate;
    class MsgObjectKnowledgeCreation;
    class MsgObjectKnowledgeUpdate;
    class MsgObjectKnowledgeDestruction;
    class MsgReport;
    class MsgPartyCreation;
    class MsgControlBeginTick;
    class MsgObjectMagicActionAck;
}

namespace MsgsMessengerToClient
{
    class MsgLimitCreation;
    class MsgLimaCreation;
}

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  DatabaseUpdater
    @brief  DatabaseUpdater
*/
// Created: JCR 2007-04-30
// =============================================================================
class DatabaseUpdater_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
            DatabaseUpdater_ABC() {}
    virtual ~DatabaseUpdater_ABC() {}
    //@}

    //! @name
    //@{
    virtual void Clean() = 0;
    //@}

    //! @name MsgsMessengerToClient
    //@{
    virtual void Update( const MsgsMessengerToClient::MsgLimitCreation& msg ) = 0;
    virtual void Update( const MsgsMessengerToClient::MsgLimaCreation& msg ) = 0;
    //@}
    

    //! @name MsgsSimToClient
    //@{
    virtual void Update( const MsgsSimToClient::MsgUnitCreation& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgUnitAttributes& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgUnitDestruction& msg ) = 0;

    virtual void Update( const MsgsSimToClient::MsgAutomatCreation& message ) = 0;
    virtual void Update( const MsgsSimToClient::MsgAutomatAttributes& msg ) = 0;

    virtual void Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg ) = 0;

    virtual void Update( const MsgsSimToClient::MsgObjectCreation& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectUpdate& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectDestruction& msg ) = 0;

    virtual void Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg ) = 0;

    virtual void Update( const MsgsSimToClient::MsgFormationCreation& message ) = 0;

    virtual void Update( const MsgsSimToClient::MsgControlBeginTick& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgPartyCreation& msg ) = 0;
    //@}
};

}
}

#endif // __crossbow_DatabaseUpdater_h_
