// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: EntityListParameter constructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityListParameter< ConcreteEntity >::EntityListParameter( QWidget* pParent, unsigned int& n, ASN1T_OID*& ids, const QString& label, const QString& menu )
    : EntityListParameterBase( pParent, n, ids, label, menu )
    , potential_             ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter destructor
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityListParameter< ConcreteEntity >::~EntityListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::NotifyDeleted
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::NotifyDeleted( const ConcreteEntity& entity )
{
    delete gui::FindItem( &entity, firstChild() );
    if( &entity == potential_ )
        potential_ = 0;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::NotifyContextMenu
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu )
{
    if( ! gui::FindItem( &entity, firstChild() ) )
    {
        potential_ = &entity;
        AddToMenu( menu );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::MenuItemValidated
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::MenuItemValidated()
{
    if( potential_ )
    {
        gui::ValuedListItem* item = new gui::ValuedListItem( this );
        item->SetNamed( *potential_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::GetId
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
unsigned long EntityListParameter< ConcreteEntity >::GetId( gui::ValuedListItem* item )
{
    return item->GetValue< const ConcreteEntity* >()->GetId();
}
