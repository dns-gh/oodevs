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

namespace sword
{
    class ObjectAttributes;
    enum ObjectMagicActionAck_ErrorCode;
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

    //! @name sword
    //@{
    void Update( const sword::LimitCreation& msg );
    void Update( const sword::LimaCreation& msg );
    //@}
    

    //! @name sword
    //@{
    void Update( const sword::UnitCreation& msg );
    void Update( const sword::UnitAttributes& msg );
    void Update( const sword::UnitDestruction& msg );

    void Update( const sword::AutomatCreation& message );
    void Update( const sword::AutomatAttributes& msg );

    void Update( const sword::UnitKnowledgeCreation& msg );
    void Update( const sword::UnitKnowledgeUpdate& msg );
    void Update( const sword::UnitKnowledgeDestruction& msg );

    void Update( const sword::ObjectCreation& msg );
    void Update( const sword::ObjectUpdate& msg );
    void Update( const sword::ObjectDestruction& msg );

    void Update( const sword::ObjectKnowledgeCreation& msg );
    void Update( const sword::ObjectKnowledgeUpdate& msg );
    void Update( const sword::ObjectKnowledgeDestruction& msg );

    void Update( const sword::FormationCreation& message );

    void Update( const sword::ControlBeginTick& msg );
    void Update( const sword::PartyCreation& msg );

    void Log( const sword::ObjectMagicActionAck& msg );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatabaseUpdater( const DatabaseUpdater& );            //!< Copy constructor
    DatabaseUpdater& operator=( const DatabaseUpdater& ); //!< Assignment operator
    //@}

    //! @name Objects
    //@{
    void UpdateObjectKnowledgeGeometry( const std::string& tablename, const sword::ObjectKnowledgeUpdate& msg );
    void UpdateObjectAttributes( unsigned long oid, const sword::ObjectAttributes& msg );
    
    // void Update( const sword::ObjectAttributes& msg );

    //@}

    //! @name Error
    //@{
    std::string Error( const sword::ObjectMagicActionAck_ErrorCode& error ) const;
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
