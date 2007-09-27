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

#include "game_asn/Asn.h"
#include "dispatcher/MessageHandler_ABC.h"

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
    class Listener_ABC;

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
             DatabasePublisher( const dispatcher::Config& config, const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher );
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

    //! @name Helpers
    //@{
    bool IsRelevant( const ASN1T_MsgsSimToClient& asn ) const;
    void UpdateListeners();
    //@}

    //! @name Types
    //@{
    typedef boost::shared_ptr< Listener_ABC >  T_SharedListener;
    typedef std::list< T_SharedListener >      T_Listeners;
    typedef T_Listeners::const_iterator      CIT_Listeners;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Workspace_ABC >      workspace_;
    std::auto_ptr< Database_ABC >       database_;                  
    std::auto_ptr< ReportFactory >      reportFactory_; // $$$$ SBO 2007-09-27: nothing to do here, maybe create a "PluginModel"
    std::auto_ptr< kernel::OrderTypes > orderTypes_;    // $$$$ SBO 2007-09-27: to put this stuff
    std::auto_ptr< DatabaseUpdater >    databaseUpdater_;
    T_Listeners                         listeners_;
    bool                                modelLoaded_;
    //@}
};

}

#endif // __DatabasePublisher_h_
