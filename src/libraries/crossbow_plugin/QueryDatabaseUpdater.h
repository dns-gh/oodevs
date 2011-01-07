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

namespace sword
{
    class Location;
    class PhaseLineCreation;
    class LimitCreation;
    class FormationCreation;
    class UnitCreation;
    class UnitKnowledgeCreation;
    class ObjectCreation;
    class Report;
    class AutomatCreation;
    class ObjectKnowledgeCreation;
    class UnitAttributes;
    class UnitKnowledgeUpdate;
    class ObjectKnowledgeUpdate;
    class AutomatAttributes;
    class UnitDestruction;
    class UnitKnowledgeDestruction;
    class ObjectDestruction;
    class ObjectKnowledgeDestruction;
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
    void Update( const sword::UnitCreation& msg );
    void Update( const sword::UnitKnowledgeCreation& msg );
    void Update( const sword::LimitCreation& msg );
    void Update( const sword::PhaseLineCreation& msg );
    void Update( const sword::ObjectCreation& msg );
    void Update( const sword::Report& msg );
    void Update( const sword::FormationCreation& message );
    void Update( const sword::AutomatCreation& message );
    void Update( const sword::ObjectKnowledgeCreation& msg );

    void Update( const sword::UnitAttributes& msg );
    void Update( const sword::UnitKnowledgeUpdate& msg );
    void Update( const sword::ObjectKnowledgeUpdate& msg );
    void Update( const sword::AutomatAttributes& msg );

    void DestroyUnit( const sword::UnitDestruction& msg );
    void DestroyUnitKnowledge( const sword::UnitKnowledgeDestruction& msg );
    void DestroyObject( const sword::ObjectDestruction& msg ); // $$$$ SBO 2007-09-27: typedef bullshit
    void DestroyObjectKnowledge( const sword::ObjectKnowledgeDestruction& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    QueryDatabaseUpdater( const QueryDatabaseUpdater& );            //!< Copy constructor
    QueryDatabaseUpdater& operator=( const QueryDatabaseUpdater& ); //!< Assignment operator
    //@}

    //! @name Helper
    //@{
    void UpdateGeometry( QueryBuilder_ABC& builder, const sword::Location& location );
    void UpdateObjectKnowledgeGeometry( const std::string& table, const sword::ObjectKnowledgeUpdate& msg );
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
