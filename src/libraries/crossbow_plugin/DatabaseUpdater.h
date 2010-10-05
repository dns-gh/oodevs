// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_DatabaseUpdater_h_
#define __crossbow_DatabaseUpdater_h_

#include "DatabaseUpdater_ABC.h"

namespace Common
{
    class ObjectAttributes;
}

namespace MsgsSimToClient
{
    enum MsgObjectMagicActionAck_ErrorCode;
}

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Database_ABC;
    class WorkingSession_ABC;

// =============================================================================
/** @class  DatabaseUpdater
    @brief  DatabaseUpdater
*/
// Created: JCR 2007-04-30
// =============================================================================
class DatabaseUpdater : public DatabaseUpdater_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DatabaseUpdater( Workspace_ABC& workspace, const dispatcher::Model_ABC& model, const WorkingSession_ABC& session );
    virtual ~DatabaseUpdater();
    //@}

    //! @name
    //@{
    void Flush( bool reset = true );
    void Clean();
    //@}

    //! @name MsgsMessengerToClient
    //@{
    void Update( const MsgsMessengerToClient::MsgLimitCreation& msg );
    void Update( const MsgsMessengerToClient::MsgLimaCreation& msg );
    //@}
    

    //! @name MsgsSimToClient
    //@{
    void Update( const MsgsSimToClient::MsgUnitCreation& msg );
    void Update( const MsgsSimToClient::MsgUnitAttributes& msg );
    void Update( const MsgsSimToClient::MsgUnitDestruction& msg );

    void Update( const MsgsSimToClient::MsgAutomatCreation& message );
    void Update( const MsgsSimToClient::MsgAutomatAttributes& msg );

    void Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg );

    void Update( const MsgsSimToClient::MsgObjectCreation& msg );
    void Update( const MsgsSimToClient::MsgObjectUpdate& msg );
    void Update( const MsgsSimToClient::MsgObjectDestruction& msg );

    void Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg );

    void Update( const MsgsSimToClient::MsgReport& msg );
    void Update( const MsgsSimToClient::MsgFormationCreation& message );
  
	void Update( const MsgsSimToClient::MsgControlBeginTick& msg );
    void Update( const MsgsSimToClient::MsgPartyCreation& msg );

    void Log( const MsgsSimToClient::MsgObjectMagicActionAck& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatabaseUpdater( const DatabaseUpdater& );            //!< Copy constructor
    DatabaseUpdater& operator=( const DatabaseUpdater& ); //!< Assignment operator
    //@}

    //! @name Objects
    //@{
    void UpdateObjectKnowledgeGeometry( const std::string& tablename, const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg );
	void UpdateObjectAttributes( unsigned long oid, const Common::ObjectAttributes& msg );
    
    // void Update( const MsgsSimToClient::MsgObjectAttributes& msg );

    //@}

    //! @name Error
    //@{
    std::string Error( const MsgsSimToClient::MsgObjectMagicActionAck_ErrorCode& error ) const;
    //@}


private:
    //! @name
    //@{
    class LazyDatabaseConnection;
    //@}

private:
    //! @name Member data
    //@{
    Workspace_ABC& workspace_;
    std::auto_ptr< LazyDatabaseConnection > database_;
    const dispatcher::Model_ABC&    model_;
    const WorkingSession_ABC&   session_;
    //@}
};

}
}

#endif // __crossbow_DatabaseUpdater_h_
