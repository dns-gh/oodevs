// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_ConnectorFacade_h_
#define __gearth_ConnectorFacade_h_

#include "game_asn/Simulation.h"
#include "game_asn/Messenger.h"
#include "dispatcher/MessageHandler_ABC.h"
#include <list>

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;
    class SimulationPublisher_ABC;
    class ClientPublisher_ABC;
    class Profile_ABC;
}

namespace plugins
{
namespace gearth
{
    class ReportFactory;
    class ExtensionFactory;

// =============================================================================
/** @class  ConnectorFacade
    @brief  ConnectorFacade
*/
// Created: JCR 2007-04-30
// =============================================================================
class ConnectorFacade : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ConnectorFacade( const dispatcher::Config& config, dispatcher::Model& model, xml::xistream& xis );
    virtual ~ConnectorFacade();
    //@}

    //! @name Operations
    //@{ 
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
            void OnReceiveMessengerToClient (const std::string&, const ASN1T_MsgsMessengerToClient&);
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ConnectorFacade( const ConnectorFacade& );            //!< Copy constructor
    ConnectorFacade& operator=( const ConnectorFacade& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{        
    void UpdateCurrentState();
    bool IsRelevant( const ASN1T_MsgsSimToClient& asn ) const;
    bool IsRelevant( const ASN1T_MsgsMessengerToClient& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ReportFactory >  reportFactory_;
    dispatcher::Model&              model_;
    const dispatcher::Config&       config_;    
    bool                            bNeedUpdate_;
    std::auto_ptr< ExtensionFactory > factory_;
    std::string                     vpath_;
    //@}
};

}
}

#endif // __gearth_ConnectorFacade_h_
