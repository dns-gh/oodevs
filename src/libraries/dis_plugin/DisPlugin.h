// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisPlugin_h_
#define __DisPlugin_h_

#include "dispatcher/Plugin_ABC.h"
#include "Time_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class Config;
}

namespace dis
{
    class UdpNetwork;
    class DisExtensionFactory;
    class DisTypeResolver;

// =============================================================================
/** @class  DisPlugin
    @brief  DisPlugin
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisPlugin : public dispatcher::Plugin_ABC, public Time_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DisPlugin( dispatcher::Model& model, const dispatcher::Config& config, xml::xistream& xis );
    virtual ~DisPlugin();
    //@}

    //! @name Operations
    //@{
    virtual void Receive                  ( const ASN1T_MsgsSimToClient& message );
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile );
    virtual void NotifyClientLeft         ( dispatcher::ClientPublisher_ABC& client );
    virtual unsigned long GetTime() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisPlugin( const DisPlugin& );            //!< Copy constructor
    DisPlugin& operator=( const DisPlugin& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Model& model_;
    std::auto_ptr< UdpNetwork > network_;
    std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    std::auto_ptr< DisTypeResolver > resolver_;
    std::auto_ptr< DisExtensionFactory > factory_;
    unsigned long timeStep_;
    unsigned long time_;
    //@}
};

}

#endif // __DisPlugin_h_
