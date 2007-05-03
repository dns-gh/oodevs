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
EntityListParameter< ConcreteEntity >::EntityListParameter( QObject* parent, const kernel::OrderParameter& parameter )
    : EntityListParameterBase( parent, parameter.GetName() )
    , potential_             ( 0 )
    , parameter_             ( parameter )
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
    delete gui::FindItem( &entity, listView_->firstChild() );
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
    if( ! gui::FindItem( &entity, listView_->firstChild() ) )
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
        gui::ValuedListItem* item = new gui::ValuedListItem( listView_ );
        item->SetNamed( *potential_ );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::GetId
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
unsigned long EntityListParameter< ConcreteEntity >::GetId( gui::ValuedListItem* item ) const
{
    return item->GetValue< const ConcreteEntity >()->GetId();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::CommitTo
// Created: SBO 2007-05-03
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::CommitTo( Action_ABC& action ) const
{
    std::auto_ptr< ActionParameter< QString > > param( new ActionParameter< QString >( parameter_ ) );
    gui::ValuedListItem* item = (gui::ValuedListItem*)( listView_->firstChild() );
    while( item )
    {
        param->AddParameter( *new ActionParameter< const ConcreteEntity* >( "entity", item->GetValue< const ConcreteEntity >() ) ); // $$$$ SBO 2007-05-03: 
        item = (gui::ValuedListItem*)( item->nextSibling() );
    }
    action.AddParameter( *param.release() );
}
