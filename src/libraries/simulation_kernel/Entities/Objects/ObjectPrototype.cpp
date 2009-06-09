// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "ObjectPrototype.h"
#include "ObjectCapacity_ABC.h"
#include "Object.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: ObjectPrototype constructor
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
ObjectPrototype::ObjectPrototype( const std::string& type, unsigned int id )
    : type_ ( type )
    , id_ ( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype destructor
// Created: JCR 2008-05-23
// -----------------------------------------------------------------------------
ObjectPrototype::~ObjectPrototype()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::GetName
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
const std::string& ObjectPrototype::GetName() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::GetID
// Created: JCR 2008-06-02
// -----------------------------------------------------------------------------
unsigned int ObjectPrototype::GetID() const
{
    return id_; 
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::GetType
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& ObjectPrototype::GetType() const
{
    return *this;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototype::Build
// Created: JCR 2008-05-26
// -----------------------------------------------------------------------------
void ObjectPrototype::Build( Object& object ) const
{
    std::for_each( capacities_.begin(), capacities_.end(), boost::bind( &ObjectCapacity_ABC::Instanciate, _1, boost::ref( object ) ) );
}
