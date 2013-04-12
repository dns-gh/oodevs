// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Object.h"
#include "tools/IdManager.h"
#include "ObjectPositions.h"
#include "Color.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include "clients_kernel/Color_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( Controller& controller, const CoordinateConverter_ABC& converter, const ObjectType& type, const QString& name, tools::IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, idManager.GetNextId(), "" )
    , converter_( converter )
    , type_     ( type )
{
    if( type.HasSpawn() )
        idManager.GetNextId(); // we need to skip one ID for dynamic created object.
//    AddExtension( *this );
    name_ = name.isEmpty() ? type.GetName().c_str() : name;
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, Controller& controller, const CoordinateConverter_ABC& converter, const kernel::ObjectType& type, tools::IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , converter_( converter )
    , type_     ( type )
{
    if( type_.HasSpawn() )
        idManager.Lock( id_ + 1 );
    else
        idManager.Lock( id_ );
    AddExtension( *this );
    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Object::~Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::~Object()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Object::GetType
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const ObjectType& Object::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetSymbol
// Created: ABR 2013-01-30
// -----------------------------------------------------------------------------
std::string Object::GetSymbol() const
{
    const ObjectPositions& pos = static_cast< const ObjectPositions& >( Get< Positions >() );
    return pos.GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: Object::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void Object::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Display( tools::translate( "Object", "Identifier:" ), id_ )
             .Display( tools::translate( "Object", "Name:" ), name_ )
             .Display( tools::translate( "Object", "Type:" ), type_ )
             .Display( tools::translate( "Object", "Location:" ), converter_.ConvertToMgrs( Get< Positions >().GetPosition() ) ); // $$$$ AGE 2006-03-22:
}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Object::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Item( "" ).Start( Styles::bold ).Add( *static_cast< Object_ABC* >( const_cast< Object* >( this ) ) ).End();
    displayer.Display( tools::translate( "Object", "Object type:" ), type_ );
}

// -----------------------------------------------------------------------------
// Name: Object::SerializeAttributes
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void Object::SerializeAttributes( xml::xostream& xos ) const
{
    gui::EntityImplementation< kernel::Object_ABC >::SerializeAttributes( xos );
    xos << xml::attribute( "type", type_.GetType() );
    if( const ::Color* color = static_cast< const ::Color* >( Retrieve< kernel::Color_ABC >() ) )
        color->SerializeAttributes( xos );
    if( const ObjectPositions* positions = static_cast< const ObjectPositions* >( Retrieve< Positions >() ) )
        positions->Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Object::CreateDictionary
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void Object::CreateDictionary()
{
    gui::PropertiesDictionary& dictionary = Get< gui::PropertiesDictionary >();
    const Entity_ABC* constEntity = static_cast< const Entity_ABC* >( this );
    dictionary.Register( *constEntity, tools::translate( "Object", "Info/Type" ), type_.GetName(), true );
}
