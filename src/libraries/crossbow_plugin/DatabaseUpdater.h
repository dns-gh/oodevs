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
    class MsgFormationCreation;
}

namespace MsgsMessengerToClient
{
    class MsgLimitCreation;
    class MsgLimaCreation;
}

namespace MsgsSimToClient
{
    class MsgUnitCreation;
    class MsgUnitKnowledgeCreation;
    class MsgLimaCreation;
    class MsgObjectCreation;
    class MsgReport;
    class MsgAutomatCreation;
    class MsgUnitAttributes;
    class MsgUnitKnowledgeUpdate;
    class MsgAutomatAttributes;
    class MsgUnitDestruction;
    class MsgUnitKnowledgeDestruction;
    class MsgObjectDestruction;
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
    class WorkingSession;

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
             DatabaseUpdater( Workspace_ABC& workspace, const dispatcher::Model_ABC& model, const WorkingSession& session );
    virtual ~DatabaseUpdater();
    //@}

    //! @name
    //@{
    void Flush();
    void Clean();
    //@}

    //! @name Operators
    //@{
    void Update( const MsgsSimToClient::MsgUnitCreation& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeCreation& msg );
    void Update( const MsgsMessengerToClient::MsgLimitCreation& msg );
    void Update( const MsgsMessengerToClient::MsgLimaCreation& msg );
    void Update( const MsgsSimToClient::MsgObjectCreation& msg );
    void Update( const MsgsSimToClient::MsgReport& msg );
    void Update( const Common::MsgFormationCreation& message );
    void Update( const MsgsSimToClient::MsgAutomatCreation& message );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg );

    void Update( const MsgsSimToClient::MsgUnitAttributes& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg );
    void Update( const MsgsSimToClient::MsgAutomatAttributes& msg );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg );

    void DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg );
    void DestroyUnitKnowledge( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg );
    void DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg ); // $$$$ SBO 2007-09-27: typedef bullshit
    void DestroyObjectKnowledge( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatabaseUpdater( const DatabaseUpdater& );            //!< Copy constructor
    DatabaseUpdater& operator=( const DatabaseUpdater& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void UpdateObjectKnowledgeGeometry( const std::string& tablename, const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg );
    //@}

private:
    //! @name Member data
    //@{
    Database_ABC&               geometryDb_;
    Database_ABC&               flatDb_;
    const dispatcher::Model_ABC&    model_;
    const WorkingSession&       session_;
    //@}
};

}
}

#endif // __crossbow_DatabaseUpdater_h_
