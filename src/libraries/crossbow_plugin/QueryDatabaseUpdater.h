// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_QueryDatabaseUpdater_h_
#define __crossbow_QueryDatabaseUpdater_h_

#include "DatabaseUpdater_ABC.h"
namespace Common
{
    class MsgLocation;
}
namespace MsgsMessengerToClient
{
    class MsgLimaCreation;
    class MsgLimitCreation;
}
namespace MsgsSimToClient
{
    class MsgFormationCreation;
    class MsgUnitCreation;
    class MsgUnitKnowledgeCreation;
    class MsgObjectCreation;
    class MsgReport;
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

namespace dispatcher
{
    class Model_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class ReportFactory;
    class WorkingSession;
    class QueryBuilder_ABC;

// =============================================================================
/** @class  QueryDatabaseUpdater
    @brief  QueryDatabaseUpdater
*/
// Created: JCR 2009-04-27
// =============================================================================
class QueryDatabaseUpdater : public DatabaseUpdater_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             QueryDatabaseUpdater( Database_ABC& database, const dispatcher::Model_ABC& model, const ReportFactory& reportFactory, const WorkingSession& session );
    virtual ~QueryDatabaseUpdater();
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
    void Update( const MsgsSimToClient::MsgFormationCreation& message );
    void Update( const MsgsSimToClient::MsgAutomatCreation& message );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& msg );

    void Update( const MsgsSimToClient::MsgUnitAttributes& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg );
    void Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg );
    void Update( const MsgsSimToClient::MsgAutomatAttributes& msg );

    void DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg );
    void DestroyUnitKnowledge( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg );
    void DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg ); // $$$$ SBO 2007-09-27: typedef bullshit
    void DestroyObjectKnowledge( const MsgsSimToClient::MsgObjectKnowledgeDestruction& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    QueryDatabaseUpdater( const QueryDatabaseUpdater& );            //!< Copy constructor
    QueryDatabaseUpdater& operator=( const QueryDatabaseUpdater& ); //!< Assignment operator
    //@}

    //! @name Helper
    //@{
    void UpdateGeometry( QueryBuilder_ABC& builder, const Common::MsgLocation& location );
    void UpdateObjectKnowledgeGeometry( const std::string& table, const MsgsSimToClient::MsgObjectKnowledgeUpdate& msg );
    //@}

private:
    //! @name Member data
    //@{
    Database_ABC&               database_;
    const dispatcher::Model_ABC&    model_;
    const ReportFactory&        reportFactory_;
    const WorkingSession&       session_;
    //@}
};

}
}

#endif // __crossbow_QueryDatabaseUpdater_h_
