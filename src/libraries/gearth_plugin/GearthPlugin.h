// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gearth_GearthPlugin_h_
#define __gearth_GearthPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace dispatcher
{
    class Model;
    class Config;    
    class SimulationPublisher_ABC;     
}

namespace gearth
{
    class ConnectorFacade;

// =============================================================================
/** @class  GearthPlugin
    @brief  GearthPlugin
*/
// Created: JCR 2007-08-29
// =============================================================================
class GearthPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GearthPlugin( const dispatcher::Model& model, const dispatcher::Config& config );
    virtual ~GearthPlugin();
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
    GearthPlugin( const GearthPlugin& );            //!< Copy constructor
    GearthPlugin& operator=( const GearthPlugin& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{    
    std::auto_ptr< ConnectorFacade > facade_;
    //@}
};
}
#endif // __gearth_GearthPlugin_h_
