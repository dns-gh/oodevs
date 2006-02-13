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
    static id = id_++;
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
void Extendable< BaseType >::AddExtension( BaseType*& where, BaseType* ext )
{
    where = ext;
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
        throw std::runtime_error( std::string( "Extension " ) + typeid( Extension ).name() + " already attached" );
    AddExtension( ext, &extension );
}

// -----------------------------------------------------------------------------
// Name: Extendable< BaseType >::Retreive
// Created: AGE 2006-02-06
// -----------------------------------------------------------------------------
template< typename BaseType >
template< typename Extension >
Extension* Extendable< BaseType >::Retreive()
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
    BaseType*& ext = Find< Extension >();
    if( ! ext )
        AddExtension( ext, new Extension() );
    return * static_cast< Extension* >( ext );
}
