// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __plugins_crossbow_CrossbowPublisher_h_
#define __plugins_crossbow_CrossbowPublisher_h_

#include "dispatcher/MessageHandler_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/shared_ptr.hpp>

namespace dispatcher
{
    class Config;
    class Model_ABC;
    class SimulationPublisher_ABC;
}

namespace sword
{
    class SimToClient;
    class MessengerToClient;
}

namespace plugins
{
namespace crossbow
{
    class OGR_Workspace;

    class DatabaseUpdater;
    class ReportUpdater;
    class FolkUpdater;
    class Listener_ABC;
    class ExtensionFactory;
    class OrderTypes;
    class WorkingSession;

// =============================================================================
/** @class  CrossbowPublisher
    @brief  CrossbowPublisher
*/
// Created: JCR 2007-04-30
// =============================================================================
class CrossbowPublisher : public dispatcher::MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPublisher( const dispatcher::Config& config, dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& publisher, xml::xistream& xis );
    virtual ~CrossbowPublisher();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& asn );
    virtual void Receive( const sword::MessengerToClient& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CrossbowPublisher( const CrossbowPublisher& );            //!< Copy constructor
    CrossbowPublisher& operator=( const CrossbowPublisher& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsRelevant( const sword::SimToClient& asn ) const;
    bool IsRelevant( const sword::MessengerToClient& asn ) const;
    bool IsRelevantAcknowledge( const sword::SimToClient& asn ) const;
    void UpdateOnTick( const sword::SimToClient& asn );
    void UpdateOnAcknowledge( const sword::SimToClient& asn );
    void UpdateDatabase( const sword::SimToClient& asn );
    void UpdateDatabase( const sword::MessengerToClient& asn );
    void UpdateFolkDatabase( const sword::SimToClient& asn );
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
    dispatcher::Model_ABC&              model_;
    std::auto_ptr< OGR_Workspace >      workspace_;
    std::auto_ptr< OrderTypes >         orderTypes_;    // $$$$ SBO 2007-09-27: to put this stuff
    std::auto_ptr< DatabaseUpdater >    databaseUpdater_;
    std::auto_ptr< ReportUpdater >      reportUpdater_;
    std::auto_ptr< FolkUpdater >        folkUpdater_;
    T_Listeners                         listeners_;
    bool                                modelLoaded_;
    std::auto_ptr< ExtensionFactory >   factory_;
    std::auto_ptr< WorkingSession >     session_;
    //@}
};

}
}

#endif // __plugins_crossbow_CrossbowPublisher_h_
