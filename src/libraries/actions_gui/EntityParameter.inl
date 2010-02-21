// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: EntityParameter constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::EntityParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : EntityParameterBase( parent, parameter.GetName().c_str() )
    , controller_        ( controller )
    , parameter_         ( parameter )
    , potential_         ( 0 )
    , selected_          ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::EntityParameter( QObject* parent, const kernel::OrderParameter& parameter, const ConcreteEntity& entity, kernel::Controller& controller )
    : EntityParameterBase( parent, parameter.GetName().c_str() )
    , controller_        ( controller )
    , parameter_         ( parameter )
    , potential_         ( &entity )
    , selected_          ( 0 )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::~EntityParameter()
{
    controller_.Unregister( *this );
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
    if( ! IsOptional() && ! selected_ )
        return Invalid();
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::CommitTo
// Created: SBO 2006-11-08
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::CommitTo( int& message ) const
{
    message = 0;
    if( ! selected_ )
    {
        if( IsOptional() )
            return;
        throw std::runtime_error( "Entity not set!" );
    }
    message = selected_->GetId();
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::CommitTo
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::CommitTo( actions::parameters::Entity< ConcreteEntity >& parameter ) const
{
    parameter.SetValue( selected_ );
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

// -----------------------------------------------------------------------------
// Name: EntityParameter::IsOptional
// Created: SBO 2008-03-10
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityParameter< ConcreteEntity >::IsOptional() const
{
    return parameter_.IsOptional();
}
