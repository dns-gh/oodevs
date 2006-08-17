// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <stdexcept>
#include <string>

template< typename BaseType >
unsigned int Extendable< BaseType >::id_ = 0;

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::GetExtensionId
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
unsigned int Extendable< BaseType >::GetExtensionId()
{
    static unsigned id = id_++;
    return id;
}

// -----------------------------------------------------------------------------
// Name: Extendable destructor
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
Extendable< BaseType >::~Extendable()
{
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::AddExtension
// Created: AGE 2006-02-07
// -----------------------------------------------------------------------------
template< typename BaseType >
void Extendable< BaseType >::AddExtension( BaseType& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::Find
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
BaseType*& Extendable< BaseType >::Find()
{
    const unsigned int id = GetExtensionId< Extension >();
    if( extensions_.size() < id + 1)
        extensions_.resize( id + 1, 0 );
    return extensions_[ id ];
}

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::Attach
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension > 
void Extendable< BaseType >::Attach( Extension& extension )
{
    BaseType*& ext = Find< Extension >();
    if( ext )
        throw std::runtime_error( std::string( "Extension " ) + typeid( Extension ).name() + " already attached to " + typeid( *this ).name() );
    AddExtension( extension );
    ext = &extension;
}

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::Retrieve
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
Extension* Extendable< BaseType >::Retrieve()
{
    return static_cast< Extension* >( Find< Extension >() );
}

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::Get
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
Extension& Extendable< BaseType >::Get()
{
    Extension* ext = Retrieve< Extension >();
    if( ! ext )
        throw std::runtime_error( std::string( "Extension " ) + typeid( Extension ).name() + " does not exist in " + typeid( *this ).name() );
    return *ext;
}

// -----------------------------------------------------------------------------
// Name: Extendable::Retrieve
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
const Extension* Extendable< BaseType >::Retrieve() const
{
    return static_cast< const Extension* >( const_cast< Extendable* >( this )->Find< Extension >() );
}

// -----------------------------------------------------------------------------
// Name: Extendable::Get
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
const Extension& Extendable< BaseType >::Get() const
{
    const Extension* ext = Retrieve< Extension >();
    if( ! ext )
        throw std::runtime_error( std::string( "Extension " ) + typeid( Extension ).name() + " does not exist in " + typeid( *this ).name() );
    return *ext;
}
