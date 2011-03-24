// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisExtension_h_
#define __DisExtension_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "rpr/EntityIdentifier.h"
#include "rpr/ForceIdentifier.h"
#include "tic_plugin/PlatformVisitor_ABC.h"
#include "protocol/Protocol.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
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
    class IdentifierFactory_ABC;

// =============================================================================
/** @class  DisExtension
    @brief  DisExtension
*/
// Created: AGE 2008-03-10
// =============================================================================
class DisExtension : public kernel::Extension_ABC
                   , public kernel::Updatable_ABC< sword::UnitAttributes >
                   , private tic::PlatformVisitor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisExtension( const Time_ABC& time, IdentifierFactory_ABC& id, const kernel::CoordinateConverter_ABC& converter, UdpNetwork& network, const rpr::EntityTypeResolver& resolver, dispatcher::Agent& holder, unsigned char exercise, bool lagAFrame );
    virtual ~DisExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& attributes );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DisExtension( const DisExtension& );            //!< Copy constructor
    DisExtension& operator=( const DisExtension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddPlatform( const tic::Platform_ABC& platform );
    //@}

    //! @name Types
    //@{
    typedef std::map< const plugins::tic::Platform_ABC*, rpr::EntityIdentifier > T_Identifiers;
    typedef T_Identifiers::iterator                                             IT_Identifiers;
    //@}

private:
    //! @name Member data
    //@{
    const Time_ABC&        time_;
    IdentifierFactory_ABC& id_;
    const kernel::CoordinateConverter_ABC& converter_;
    UdpNetwork&            network_;
    const rpr::EntityTypeResolver& resolver_;
    dispatcher::Agent&     holder_;
    rpr::ForceIdentifier   forceId_;
    const unsigned char    exercise_;
    bool                   lagAFrame_;
    T_Identifiers          ids_;
    std::auto_ptr< tic::Platform_ABC > adapted_;
    //@}
};

}
}

#endif // __DisExtension_h_
