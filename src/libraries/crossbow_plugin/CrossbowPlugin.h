// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossbowPlugin_h_
#define __CrossbowPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace dispatcher
{
    class Model;
    class Config;    
    class SimulationPublisher_ABC;     
}

namespace crossbow
{
    class ConnectorFacade;

// =============================================================================
/** @class  CrossbowPlugin
    @brief  CrossbowPlugin
*/
// Created: JCR 2007-08-29
// =============================================================================
class CrossbowPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             CrossbowPlugin( const dispatcher::Model& model, const dispatcher::Config& config, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~CrossbowPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& asnMsg );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossbowPlugin( const CrossbowPlugin& );            //!< Copy constructor
    CrossbowPlugin& operator=( const CrossbowPlugin& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ConnectorFacade > facade_;
    //@}
};
}
#endif // __CrossbowPlugin_h_
