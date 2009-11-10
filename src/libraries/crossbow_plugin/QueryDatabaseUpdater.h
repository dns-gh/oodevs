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

#include "game_asn/Simulation.h"

namespace dispatcher
{
    class Model;
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
class QueryDatabaseUpdater
{
public:
    //! @name Constructors/Destructor
    //@{
             QueryDatabaseUpdater( Database_ABC& database, const dispatcher::Model& model, const ReportFactory& reportFactory, const WorkingSession& session );
    virtual ~QueryDatabaseUpdater();
    //@}

    //! @name 
    //@{
    void Flush();    
    void Clean();
    //@}

    //! @name Operators
    //@{
	void Update( const ASN1T_MsgUnitCreation& msg );
    void Update( const ASN1T_MsgUnitKnowledgeCreation& msg );
    void Update( const ASN1T_MsgLimitCreation& msg );
    void Update( const ASN1T_MsgLimaCreation& msg );
    void Update( const ASN1T_MsgObjectCreation& msg );
    void Update( const ASN1T_MsgReport& msg );
    void Update( const ASN1T_MsgFormationCreation& asn );
    void Update( const ASN1T_MsgAutomatCreation& asn );
	void Update( const ASN1T_MsgObjectKnowledgeCreation& msg );

    void Update( const ASN1T_MsgUnitAttributes& msg );
    void Update( const ASN1T_MsgUnitKnowledgeUpdate& msg );
	void Update( const ASN1T_MsgObjectKnowledgeUpdate& msg );
    void Update( const ASN1T_MsgAutomatAttributes& msg );

    void DestroyUnit( const ASN1T_MsgUnitDestruction& msg );
    void DestroyUnitKnowledge( const ASN1T_MsgUnitKnowledgeDestruction& msg );
    void DestroyObject( const ASN1T_MsgObjectDestruction& msg ); // $$$$ SBO 2007-09-27: typedef bullshit
	void DestroyObjectKnowledge( const ASN1T_MsgObjectKnowledgeDestruction& msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    QueryDatabaseUpdater( const QueryDatabaseUpdater& );            //!< Copy constructor
    QueryDatabaseUpdater& operator=( const QueryDatabaseUpdater& ); //!< Assignement operator
    //@}

    //! @name Helper
    //@{
    void UpdateGeometry( QueryBuilder_ABC& builder, const ASN1T_Location& location );
    void UpdateObjectKnowledgeGeometry( const std::string& table, const ASN1T_MsgObjectKnowledgeUpdate& msg );
    //@}

private:
    //! @name Member data
    //@{
    Database_ABC&               database_;
    const dispatcher::Model&    model_;
    const ReportFactory&        reportFactory_;    
    const WorkingSession&       session_;
    //@}
};

}
}

#endif // __crossbow_QueryDatabaseUpdater_h_
