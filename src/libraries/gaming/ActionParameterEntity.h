// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterEntity_h_
#define __ActionParameterEntity_h_

#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"
#include "xeumeuleu/xml.h"

// =============================================================================
/** @class  ActionParameterEntity
    @brief  ActionParameterEntity
*/
// Created: SBO 2007-05-04
// =============================================================================
template< typename ConcreteEntity >
class ActionParameterEntity : public ActionParameter< const ConcreteEntity* >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterEntity( const QString& name, const ConcreteEntity* entity );
             ActionParameterEntity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity );
             ActionParameterEntity( const kernel::OrderParameter& parameter, unsigned long id, const kernel::Resolver_ABC< ConcreteEntity >& resolver );
             ActionParameterEntity( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< ConcreteEntity >& resolver );
    virtual ~ActionParameterEntity();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterEntity( const ActionParameterEntity& );            //!< Copy constructor
    ActionParameterEntity& operator=( const ActionParameterEntity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const QString& name, const ConcreteEntity* entity )
    : ActionParameter< const ConcreteEntity* >( name, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity )
    : ActionParameter< const ConcreteEntity* >( parameter, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const kernel::OrderParameter& parameter, unsigned long id, const kernel::Resolver_ABC< ConcreteEntity >& resolver )
    : ActionParameter< const ConcreteEntity* >( parameter, &resolver.Get( id ) )
{
    // NOTHING
}

namespace
{
    unsigned long ReadId( xml::xistream& xis )
    {
        unsigned long id;
        xis >> xml::attribute( "value", id );
        return id;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity constructor
// Created: SBO 2007-05-21
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< ConcreteEntity >& resolver )
    : ActionParameter< const ConcreteEntity* >( parameter, &resolver.Get( ReadId( xis ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::~ActionParameterEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity::Serialize
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntity< ConcreteEntity >::Serialize( xml::xostream& xos ) const
{
    ActionParameter< const ConcreteEntity* >::Serialize( xos );
    xos << xml::attribute( "value", GetValue()->GetId() );
}

#endif // __ActionParameterEntity_h_
