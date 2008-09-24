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
    void Lock();
    void UnLock();
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

    void Update( const ASN1T_MsgUnitAttributes& msg );
    void Update( const ASN1T_MsgUnitKnowledgeUpdate& msg );
    void Update( const ASN1T_MsgAutomatAttributes& msg );

    void DestroyUnit( const ASN1T_MsgUnitDestruction& msg );
    void Update( const ASN1T_MsgUnitKnowledgeDestruction& msg );
    void DestroyObject( const ASN1T_MsgObjectDestruction& msg ); // $$$$ SBO 2007-09-27: typedef bullshit
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
