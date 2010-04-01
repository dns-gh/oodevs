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
    class MsgUnitCreation;
    class MsgUnitKnowledgeCreation;
    class MsgObjectCreation;
    class MsgAutomatCreation;
    class MsgObjectKnowledgeCreation;
    class MsgUnitAttributes;
    class MsgUnitKnowledgeUpdate;
    class MsgObjectKnowledgeUpdate;
    class MsgAutomatAttributes;
    class MsgUnitDestruction;
    class MsgUnitKnowledgeDestruction;
    class MsgObjectDestruction;
    class MsgObjectKnowledgeDestruction;
}

namespace MsgsMessengerToClient
{
    class MsgLimitCreation;
    class MsgLimaCreation;
}

namespace Common
{
    class MsgFormationCreation;
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

    //! @name Operators
    //@{
    virtual void Update( const MsgsSimToClient::MsgUnitCreation& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg ) = 0;
    virtual void Update( const MsgsMessengerToClient::MsgLimitCreation& msg ) = 0;
    virtual void Update( const MsgsMessengerToClient::MsgLimaCreation& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectCreation& msg ) = 0;
    virtual void Update( const Common::MsgFormationCreation& asn ) = 0;
    virtual void Update( const MsgsSimToClient::MsgAutomatCreation& asn ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg ) = 0;

    virtual void Update( const MsgsSimToClient::MsgUnitAttributes& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg ) = 0;
    virtual void Update( const MsgsSimToClient::MsgAutomatAttributes& msg ) = 0;

    virtual void DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg ) = 0;
    virtual void DestroyUnitKnowledge( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg ) = 0;
    virtual void DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg ) = 0; // $$$$ SBO 2007-09-27: typedef bullshit
    virtual void DestroyObjectKnowledge( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg ) = 0;
    //@}
};

}
}

#endif // __crossbow_DatabaseUpdater_h_
