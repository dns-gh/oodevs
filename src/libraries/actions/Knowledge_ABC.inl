// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC constructor
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Knowledge_ABC< ConcreteEntity >::Knowledge_ABC(const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : Entity< ConcreteEntity >( parameter, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC constructor
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Knowledge_ABC< ConcreteEntity >::Knowledge_ABC(const kernel::OrderParameter& parameter, const ConcreteEntity* entity, kernel::Controller& controller )
    : Entity< ConcreteEntity >( parameter, entity, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC destructor
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Knowledge_ABC< ConcreteEntity >::~Knowledge_ABC()
{
    if( GetValue() )
        const_cast< ConcreteEntity* >( GetValue() )->RemoveListener( *this );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::SetValue
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::SetValue( const ConcreteEntity*& value )
{
    Entity< ConcreteEntity >::SetValue( value );
    if( ! GetValue() )
        ThrowInvalidKnowledge();
    const_cast< ConcreteEntity* >( GetValue() )->AddListener( *this );
    id_ = GetValue()->GetEntity()->GetId();
    isEntityValid_ = true;
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::Display
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::Display( kernel::Displayer_ABC& displayer ) const
{
    if( isEntityValid_ )
        Entity< ConcreteEntity >::Display( displayer );
    else
        displayer.Item( tools::translate( "Parameter", "Parameter" ) ).Display( GetName() )
                 .Item( tools::translate( "Parameter", "Value" ) ).Display( id_ );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::NotifyDestruction
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::NotifyDestruction()
{
    isEntityValid_ = false;
}

// -----------------------------------------------------------------------------
// Name: Knowledge_ABC::Serialize
// Created: JSR 2010-05-20
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void Knowledge_ABC< ConcreteEntity >::Serialize( xml::xostream& xos ) const
{
    if( ! GetValue() )
        ThrowInvalidKnowledge();
    Parameter< const ConcreteEntity* >::Serialize( xos );
    xos << xml::attribute( "value", id_ ); // $$$$ SBO 2007-05-24: 
}
