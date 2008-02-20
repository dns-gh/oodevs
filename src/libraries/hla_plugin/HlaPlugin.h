// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HlaPlugin_h_
#define __HlaPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace dispatcher
{
    class Model;
    class Config;
}

namespace hla
{
    class FederateFacade;

// =============================================================================
/** @class  HlaPlugin
    @brief  HlaPlugin
*/
// Created: SBO 2008-02-18
// =============================================================================
class HlaPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HlaPlugin( dispatcher::Model& model, const dispatcher::Config& config );
    virtual ~HlaPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HlaPlugin( const HlaPlugin& );            //!< Copy constructor
    HlaPlugin& operator=( const HlaPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FederateFacade > federate_;
    //@}
};

}

#endif // __HlaPlugin_h_
