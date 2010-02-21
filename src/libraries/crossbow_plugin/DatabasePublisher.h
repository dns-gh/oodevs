// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DatabasePublisher_h_
#define __DatabasePublisher_h_

#include "dispatcher/MessageHandler_ABC.h"
#include <xeumeuleu/xml.h>

namespace MsgsSimToClient
{
    class MsgSimToClient;
}

namespace MsgsMessengerToClient
{
    class MsgMessengerToClient;
}


namespace dispatcher
{
    class Config;
    class Model;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Database_ABC;
    class ReportFactory;
    class QueryDatabaseUpdater;
    class FolkUpdater;
    class Listener_ABC;
    class ExtensionFactory;
    class OrderTypes;
    class WorkingSession;

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
    virtual void Receive( const MsgsSimToClient::MsgSimToClient& message );
    virtual void Receive( const MsgsMessengerToClient::MsgMessengerToClient& message );
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
    bool IsRelevant( const MsgsSimToClient::MsgSimToClient& message ) const;
    bool IsRelevant( const MsgsMessengerToClient::MsgMessengerToClient& message ) const;
    void UpdateOnTick( const MsgsSimToClient::MsgSimToClient& message );
    void UpdateDatabase( const MsgsSimToClient::MsgSimToClient& message );
    void UpdateDatabase( const MsgsMessengerToClient::MsgMessengerToClient& message );
    void UpdateFolkDatabase( const MsgsSimToClient::MsgSimToClient& message );
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
    std::auto_ptr< OrderTypes >         orderTypes_;    // $$$$ SBO 2007-09-27: to put this stuff
    std::auto_ptr< QueryDatabaseUpdater >    databaseUpdater_;
    std::auto_ptr< FolkUpdater >        folkUpdater_;
    T_Listeners                         listeners_;
    bool                                modelLoaded_;
    std::auto_ptr< ExtensionFactory >   factory_;
    std::auto_ptr< WorkingSession >     session_;
    //@}
};

}
}

#endif // __DatabasePublisher_h_
