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
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::EntityParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : EntityParameterBase( builder, parameter )
    , potential_         ( 0 )
    , selected_          ( 0 )
{
    controllers_.controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityParameter< ConcreteEntity >::~EntityParameter()
{
    Purge();
    controllers_.controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::Purge
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::Purge()
{
    potential_ = 0;
    selected_ = 0;
    Display( "---" );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu )
{
    potential_ = &entity;
    CreateMenu( menu );
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
        throw MASA_EXCEPTION( "Entity not set!" );
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
void EntityParameter< ConcreteEntity >::OnMenuClick()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName() : "---" );
    if( group_ && IsOptional() )
        group_->setChecked( selected_ != 0 );
    NotifyChange();
    Update();
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
// Name: EntityParameter::SetPotential
// Created: ABR 2012-01-04
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::SetPotential( const ConcreteEntity& entity )
{
    potential_ = &entity;
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::InternalVisit
// Created: ABR 2013-06-13
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::InternalVisit( const actions::parameters::Entity< ConcreteEntity >& param )
{
    ActivateOptionalIfNeeded( param );
    potential_ = param.GetValue();
    OnMenuClick();
}
