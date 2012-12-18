// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Controller.h"
#include "InstanciationComplete.h"

namespace kernel
{

// -----------------------------------------------------------------------------
// Name: EntityImplementation constructor
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
EntityImplementation< I >::EntityImplementation( Controller& controller, unsigned long id, const QString& name )
    : controller_( controller )
    , id_        ( id )
    , name_      ( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation destructor
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
EntityImplementation< I >::~EntityImplementation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::GetName
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
QString EntityImplementation< I >::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Rename
// Created: JSR 2012-12-18
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Rename( const QString& name )
{
    if( name_ != name )
    {
        name_ = name;
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::GetId
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
unsigned long EntityImplementation< I >::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Polish
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Polish()
{
    Update( InstanciationComplete() );
    controller_.Create( This() );
    controller_.Create( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Touch
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Touch()
{
    controller_.Update( This() );
    controller_.Update( *(Entity_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::Destroy
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
void EntityImplementation< I >::Destroy()
{
    controller_.Delete( This() );
    controller_.Delete( *(Entity_ABC*)this );
    DestroyExtensions();
}

// -----------------------------------------------------------------------------
// Name: EntityImplementation::This
// Created: AGE 2006-10-12
// -----------------------------------------------------------------------------
template< typename I >
I& EntityImplementation< I >::This()
{
    return *this;
}

}