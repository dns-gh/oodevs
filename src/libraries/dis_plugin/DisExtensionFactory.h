// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisExtensionFactory_h_
#define __DisExtensionFactory_h_

#include "dispatcher/ExtensionFactory_ABC.h"
#include "IdentifierFactory_ABC.h"

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
    class Agent;
    class Automat;
    class Formation;
    class Side;
}

namespace rpr
{
    class EntityTypeResolver;
}

namespace plugins
{
namespace dis
{
    class UdpNetwork;
    class Time_ABC;

// =============================================================================
/** @class  DisExtensionFactory
    @brief  DisExtensionFactory
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisExtensionFactory : public dispatcher::ExtensionFactory_ABC< dispatcher::Agent >
                          , private IdentifierFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisExtensionFactory( UdpNetwork& network, const Time_ABC& time, const kernel::CoordinateConverter_ABC& converter, const rpr::EntityTypeResolver& resolver, xml::xistream& xis );
    virtual ~DisExtensionFactory();
    //@}

    //! @name Operations
    //@{
    virtual void Create( dispatcher::Agent&     entity );
    virtual rpr::EntityIdentifier CreateNewIdentifier();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisExtensionFactory( const DisExtensionFactory& );            //!< Copy constructor
    DisExtensionFactory& operator=( const DisExtensionFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    UdpNetwork& network_;
    const Time_ABC& time_;
    const kernel::CoordinateConverter_ABC& converter_;
    const rpr::EntityTypeResolver& resolver_;
    const unsigned short site_;
    const unsigned short application_;
    const unsigned char  exercise_;
    const bool lag_;
    unsigned short id_;
    //@}
};

}
}

#endif // __DisExtensionFactory_h_
