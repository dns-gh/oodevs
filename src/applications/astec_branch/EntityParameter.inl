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
EntityParameter< ConcreteEntity >::EntityParameter( QWidget* pParent, ASN1T_OID& id, const std::string& label, const std::string& menu )
    : EntityParameterBase( pParent, id, label, menu )
    , potential_         ( 0 )
    , selected_          ( 0 )
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
void EntityParameter< ConcreteEntity >::NotifyContextMenu( const ConcreteEntity& entity, QPopupMenu& menu )
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
// Name: EntityParameter::Commit
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::Commit()
{
    if( ! selected_ )
        throw std::runtime_error( "Entity not set!" );
    SetId( selected_->GetId() );
}

// -----------------------------------------------------------------------------
// Name: EntityParameter::MenuItemValidated
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityParameter< ConcreteEntity >::MenuItemValidated()
{
    selected_ = potential_;
    Display( selected_ ? selected_->GetName().c_str() : "---" ); // $$$$ AGE 2006-03-14: use a displayer
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
