// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Connector_h_
#define __Connector_h_

#include "ESRI.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class Config;
    class Model;
    class ModelVisitor_ABC;
}

namespace crossbow
{
    class Database_ABC;
    class Table_ABC;
    class ScopeEditor;
    class ReportFactory;
    class FolkEditor;

// =============================================================================
/** @class  Connector
    @brief  Connector
*/
// Created: JCR 2007-04-30
// =============================================================================
class Connector : public dispatcher::ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Connector( const dispatcher::Config& config, const dispatcher::Model& model );
    virtual ~Connector();
    //@}

    //! @name Operations
    //@{
    void Lock();
    void Unlock();
    Table_ABC& GetTable( const std::string& name, bool clear = true );
    virtual void Send( const ASN1T_MsgsSimToClient& msg );
    virtual void Send( const ASN1T_MsgsAuthenticationToClient& ) {}
    virtual void Send( const ASN1T_MsgsReplayToClient&         ) {}
    virtual void Send( const ASN1T_MsgsAarToClient&            ) {}
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Connector( const Connector& );            //!< Copy constructor
    Connector& operator=( const Connector& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CheckOutLicences( esriLicenseProductCode eProcuct );
    Table_ABC& GetTable( const ASN1T_Location& location );
    void Delete( const ASN1T_MsgObjectDestruction& msg );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, Table_ABC* > T_Tables;
    //@}

private:
    //! @name Member data
    //@{
    IAoInitializePtr               spLicenseHandler_;
    std::auto_ptr< Database_ABC >  database_;                  
    const dispatcher::Model&       model_;
    std::auto_ptr< ReportFactory > reportFactory_;
    std::auto_ptr< FolkEditor >    folkEditor_;
    std::auto_ptr< ScopeEditor >   scopeEditor_;
    T_Tables                       tables_;
    //@}
};

}

#endif // __Connector_h_
