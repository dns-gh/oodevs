// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TicPlugin_h_
#define __TicPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace dispatcher
{
    class Model;
    class Config;
}

namespace kernel
{
    class CoordinateConverter;
}

namespace tic
{
    class ExtensionFactory;

// =============================================================================
/** @class  TicPlugin
    @brief  TicPlugin
*/
// Created: AGE 2008-04-01
// =============================================================================
class TicPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TicPlugin( dispatcher::Model& model, const dispatcher::Config& config );
    virtual ~TicPlugin();
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
    TicPlugin( const TicPlugin& );            //!< Copy constructor
    TicPlugin& operator=( const TicPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< kernel::CoordinateConverter > converter_;
    std::auto_ptr< ExtensionFactory > factory_;
    //@}
};

}

#endif // __TicPlugin_h_
