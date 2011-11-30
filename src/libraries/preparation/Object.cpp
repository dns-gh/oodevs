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
#include "IdManager.h"
#include "ObjectPositions.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( Controller& controller, const CoordinateConverter_ABC& converter, const ObjectType& type, const QString& name, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, idManager.GetNextId(), "" )
    , converter_( converter )
    , type_     ( type )
{
    if( type.HasSpawn() )
        idManager.GetNextId(); // we need to skip one ID for dynamic created object.
    RegisterSelf( *this );
    name_ = name.isEmpty() ? tools::translate( "Object", "%1 [%2]" ).arg( type.GetName().c_str() ).arg( id_ ) : name;
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, Controller& controller, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , converter_( converter )
    , type_     ( types.Get( xis.attribute< std::string >( "type" ) ) )
{
    if( type_.HasSpawn() )
        idManager.Lock( id_ + 1 );
    else
        idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
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
    xos << xml::attribute( "id", static_cast< long >( id_ ) )
        << xml::attribute( "type", type_.GetType() )
        << xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: Object::SerializePositions
// Created: JSR 2011-02-24
// -----------------------------------------------------------------------------
void Object::SerializePositions( xml::xostream& xos ) const
{
    if( const ObjectPositions* positions = static_cast< const ObjectPositions* >( Retrieve< Positions >() ) )
        positions->Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: Object::CreateDictionary
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void Object::CreateDictionary( Controller& controller )
{
    PropertiesDictionary& dico = *new PropertiesDictionary( controller );
    Attach( dico );
    const Object& constSelf = *this;
    const Entity_ABC* constEntity = static_cast< const Entity_ABC* >( this );
    dico.Register( *constEntity, tools::translate( "Object", "Info/Identifier" ), constSelf.id_ );
    dico.Register( *constEntity, tools::translate( "Object", "Info/Name" ), name_ );
    dico.Register( *constEntity, tools::translate( "Object", "Info/Type" ), type_.GetName() );
}
