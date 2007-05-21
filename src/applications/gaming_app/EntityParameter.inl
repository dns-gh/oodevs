// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: EntityParameter constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::EntityParameter( QObject* parent, const kernel::OrderParameter& parameter )
    : EntityParameterBase( parent, parameter.GetName() )
    , parameter_         ( &parameter )
    , potential_         ( 0 )
    , selected_          ( 0 )
    , optional_          ( parameter.IsOptional() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityParameter constructor
// Created: SBO 2007-04-25
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::EntityParameter( QObject* parent, const QString& name, bool optional )
    : EntityParameterBase( parent, name )
    , parameter_         ( 0 )
    , potential_         ( 0 )
    , selected_          ( 0 )
    , optional_          ( optional )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityParameter destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::~EntityParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu )
{
    potential_ = &entity;
    AddToMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::CheckValidity
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityParameter< ConcreteEntity >::CheckValidity()
{
    if( ! optional_ && ! selected_ )
        return Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::CommitTo
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::CommitTo( ASN1T_OID& asn ) const
{
    asn = 0;
    if( ! selected_ )
    {
        if( optional_ )
            return;
        throw std::runtime_error( "Entity not set!" );
    }
    asn = selected_->GetId();
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::CommitTo( Action_ABC& action ) const
{
    if( !selected_ )
        return;
    if( !parameter_ )
        throw std::runtime_error( "OrderParameter not set" ); // $$$$ SBO 2007-04-25: 
    action.AddParameter( *new ActionParameterEntity< ConcreteEntity >( *parameter_, selected_ ) );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::CommitTo
// Created: SBO 2007-04-26
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::CommitTo( ActionParameter_ABC& parameter ) const
{
    if( !selected_ )
        return;
    parameter.AddParameter( *new ActionParameterEntity< ConcreteEntity >( GetName(), selected_ ) );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::MenuItemValidated
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::MenuItemValidated()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName() : "---" ); // $$$$ AGE 2006-03-14: use a displayer
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::NotifyDeleted
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::NotifyDeleted( const ConcreteEntity& entity )
{
    if( &entity == potential_ )
        potential_ = 0;
    if( &entity == selected_ )
    {
        selected_ = 0;
        Display( "---" );
    }
}
