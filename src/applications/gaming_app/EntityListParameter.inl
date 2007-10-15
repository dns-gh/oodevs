// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: EntityListParameter constructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityListParameter< ConcreteEntity >::EntityListParameter( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& controller )
    : EntityListParameterBase( parent, parameter, controller )
    , potential_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter destructor
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
EntityListParameter< ConcreteEntity >::~EntityListParameter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::Add
// Created: AGE 2007-10-15
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::Add( const ConcreteEntity& entity )
{
    potential_ = &entity;
    MenuItemValidated();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::MenuItemValidated
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::MenuItemValidated()
{
    OnCreate();
    CIT_Entities it = entities_.find( potential_ );
    if( it != entities_.end() )
        it->second->MenuItemValidated();
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::CreateElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
Param_ABC* EntityListParameter< ConcreteEntity >::CreateElement()
{
    if( !potential_ )
        return 0;
    EntityParameter< ConcreteEntity >* param = CreateElement( *potential_ );
    entities_[potential_] = param;
    return param;
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::DeleteElement
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::DeleteElement( Param_ABC& param )
{
    for( T_Entities::iterator it = entities_.begin(); it != entities_.end(); ++it )
        if( it->second == &param )
        {
            entities_.erase( it );
            break;
        }
    EntityListParameterBase::DeleteElement( param );
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::NotifyContextMenu
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::NotifyContextMenu( const ConcreteEntity& entity, kernel::ContextMenu& menu )
{
    CIT_Entities it = entities_.find( &entity );
    if( it != entities_.end() )
        Select( *it->second );
    else
    {
        potential_ = &entity;
        AddToMenu( menu );
    }
}

// -----------------------------------------------------------------------------
// Name: EntityListParameter::NotifyDeleted
// Created: SBO 2007-05-23
// -----------------------------------------------------------------------------
template< typename ConcreteEntity >
void EntityListParameter< ConcreteEntity >::NotifyDeleted( const ConcreteEntity& entity )
{
    CIT_Entities it = entities_.find( &entity );
    if( it != entities_.end() )
    {
        Select( *it->second );
        OnDeleteSelectedItem();
    }
}
