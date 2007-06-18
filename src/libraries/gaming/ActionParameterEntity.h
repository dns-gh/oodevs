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
    explicit ActionParameterEntity( const kernel::OrderParameter& parameter );
             ActionParameterEntity( const kernel::OrderParameter& parameter, const ConcreteEntity* entity );
    virtual ~ActionParameterEntity();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_OID& oid ) const;
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
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::ActionParameterEntity( const kernel::OrderParameter& parameter )
    : ActionParameter< const ConcreteEntity* >( parameter )
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
// Name: ActionParameterEntity destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
ActionParameterEntity< ConcreteEntity >::~ActionParameterEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterEntity::CommitTo
// Created: SBO 2007-05-22
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void ActionParameterEntity< ConcreteEntity >::CommitTo( ASN1T_OID& oid ) const
{
    oid = GetValue()->GetId();
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
