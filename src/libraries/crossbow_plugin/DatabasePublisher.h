// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DatabasePublisher_h_
#define __DatabasePublisher_h_

#include "game_asn/Simulation.h"
#include "dispatcher/MessageHandler_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class OrderTypes;
}

namespace dispatcher
{
    class Config;
    class Model;
    class SimulationPublisher_ABC;
}

namespace crossbow
{
    class Workspace_ABC;
    class Database_ABC;
    class ReportFactory;
    class DatabaseUpdater;
    class FolkUpdater;
    class Listener_ABC;
    class ExtensionFactory;

// =============================================================================
/** @class  DatabasePublisher
    @brief  DatabasePublisher
*/
// Created: JCR 2007-04-30
// =============================================================================
class DatabasePublisher : public dispatcher::MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DatabasePublisher( const dispatcher::Config& config, dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis );
    virtual ~DatabasePublisher();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& asn );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DatabasePublisher( const DatabasePublisher& );            //!< Copy constructor
    DatabasePublisher& operator=( const DatabasePublisher& ); //!< Assignement operator
    //@}

    //! @name Database Initializer
    //@{
    void Initialize( const std::string& name, const dispatcher::Config& config, xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    bool IsRelevant( const ASN1T_MsgsSimToClient& asn ) const;
    void UpdateOnTick( const ASN1T_MsgsSimToClient& asn );
    void UpdateDatabase( const ASN1T_MsgsSimToClient& asn );
    void UpdateFolkDatabase( const ASN1T_MsgsSimToClient& asn );
    void UpdateListeners();
    //@}

    //! @name Types
    //@{
    typedef boost::shared_ptr< Listener_ABC >  T_SharedListener;
    typedef std::list< T_SharedListener >      T_Listeners;
    typedef T_Listeners::const_iterator      CIT_Listeners;
    
    typedef boost::shared_ptr< Database_ABC >           T_SharedDatabase;
    typedef std::map< std::string, T_SharedDatabase >   T_Databases;    
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model&                  model_;
    std::auto_ptr< Workspace_ABC >      workspace_;
    T_Databases                         databases_;
    std::auto_ptr< ReportFactory >      reportFactory_; // $$$$ SBO 2007-09-27: nothing to do here, maybe create a "PluginModel"
    std::auto_ptr< kernel::OrderTypes > orderTypes_;    // $$$$ SBO 2007-09-27: to put this stuff
    std::auto_ptr< DatabaseUpdater >    databaseUpdater_;
    std::auto_ptr< FolkUpdater >        folkUpdater_;
    T_Listeners                         listeners_;
    bool                                modelLoaded_;
    std::auto_ptr< ExtensionFactory >   factory_;
    //@}
};

}

#endif // __DatabasePublisher_h_
