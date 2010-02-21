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
    class Model;
}

namespace plugins
{
namespace crossbow
{
    class Database_ABC;
    class ReportFactory;
    
// =============================================================================
/** @class  DatabaseUpdater
    @brief  DatabaseUpdater
*/
// Created: JCR 2007-04-30
// =============================================================================
class DatabaseUpdater
{
public:
    //! @name Constructors/Destructor
    //@{
             DatabaseUpdater( Database_ABC& database, const dispatcher::Model& model, const ReportFactory& reportFactory );
    virtual ~DatabaseUpdater();
    //@}

    //! @name 
    //@{
    void Flush();    
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

    void Update( const MsgsSimToClient::MsgUnitAttributes& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeUpdate& msg );
    void Update( const MsgsSimToClient::MsgAutomatAttributes& msg );

    void DestroyUnit( const MsgsSimToClient::MsgUnitDestruction& msg );
    void Update( const MsgsSimToClient::MsgUnitKnowledgeDestruction& msg );
    void DestroyObject( const MsgsSimToClient::MsgObjectDestruction& msg ); // $$$$ SBO 2007-09-27: typedef bullshit
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DatabaseUpdater( const DatabaseUpdater& );            //!< Copy constructor
    DatabaseUpdater& operator=( const DatabaseUpdater& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Database_ABC&                database_;
    const dispatcher::Model&     model_;
    const ReportFactory&         reportFactory_;    
    //@}
};

}
}

#endif // __crossbow_DatabaseUpdater_h_
