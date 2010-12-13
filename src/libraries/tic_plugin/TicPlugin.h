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
#include "protocol/protocol.h"

namespace dispatcher
{
    class Model_ABC;
    class Config;
}

namespace kernel
{
    class CoordinateConverter;
}

namespace plugins
{
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
             TicPlugin( dispatcher::Model_ABC& model, const dispatcher::Config& config );
    virtual ~TicPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const sword::SimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& client );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TicPlugin( const TicPlugin& );            //!< Copy constructor
    TicPlugin& operator=( const TicPlugin& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model_ABC& model_;
    std::auto_ptr< kernel::CoordinateConverter > converter_;
    std::auto_ptr< ExtensionFactory > factory_;
    //@}
};

}
}

#endif // __TicPlugin_h_
