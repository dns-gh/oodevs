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
EntityParameter< ConcreteEntity >::EntityParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::Controller& controller )
    : EntityParameterBase( parent, parameter.GetName().c_str(), parameter.IsOptional() )
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
    : EntityParameterBase( parent, parameter.GetName().c_str(), parameter.IsOptional() )
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
// Name: EntityParameter::InternalCheckValidity
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
bool EntityParameter< ConcreteEntity >::InternalCheckValidity() const
{
    return selected_ != 0;
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
    if( IsChecked() && selected_ )
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
    Display( selected_ ? selected_->GetName() : "---" );
    if( group_ && IsOptional() )
        group_->setChecked( selected_ != 0 );
    NotifyChange();
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
