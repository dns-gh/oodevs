// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectAttributePrototypeFactory.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeFactory::ObjectAttributePrototypeFactory
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeFactory::ObjectAttributePrototypeFactory()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeFactory::~ObjectAttributePrototypeFactory
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
ObjectAttributePrototypeFactory::~ObjectAttributePrototypeFactory()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeFactory::Register
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeFactory::Register( const std::string& capacity, const T_CallBack& callback )
{
    if ( ! callbacks_.insert( std::make_pair( capacity, callback ) ).second )
        throw std::invalid_argument( "capacity '" + capacity + "' already registered." );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectAttributePrototypeFactory::Create
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void ObjectAttributePrototypeFactory::Create( const std::string& capacity, xml::xistream& xis, T_AttributeContainer& container, QWidget* parent ) const
{
    T_CallBacks::const_iterator it = callbacks_.find( capacity );
    if ( it != callbacks_.end() )
        it->second( xis, container, parent );
}
