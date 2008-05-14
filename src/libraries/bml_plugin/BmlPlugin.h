// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BmlPlugin_h_
#define __BmlPlugin_h_

#include "dispatcher/Plugin_ABC.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;
}

namespace bml
{
    class ExtensionFactory;
    class Publisher;

// =============================================================================
/** @class  BmlPlugin
    @brief  BmlPlugin
*/
// Created: SBO 2008-02-29
// =============================================================================
class BmlPlugin : public dispatcher::Plugin_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BmlPlugin( dispatcher::Model& model, const dispatcher::Config& config, xml::xistream& xis );
    virtual ~BmlPlugin();
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
    BmlPlugin( const BmlPlugin& );            //!< Copy constructor
    BmlPlugin& operator=( const BmlPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
    std::auto_ptr< Publisher > publisher_;
    std::auto_ptr< ExtensionFactory > factory_;
    //@}
};

}

#endif // __BmlPlugin_h_
