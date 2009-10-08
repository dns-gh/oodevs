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
#include "ObjectHierarchies.h"
#include "Team.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Styles.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------    
Object::Object( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const kernel::ObjectType& type, const QString& name, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, idManager.GetNextId(), "" )
    , converter_( converter )
    , type_( type )
{
    RegisterSelf( *this );
    name_ = name.isEmpty() ? tools::translate( "Object", "%1 [%2]" ).arg( type.GetName().c_str() ).arg( id_ ) : name;
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const tools::Resolver_ABC< ObjectType, std::string >& types, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, xml::attribute< unsigned long >( xis, "id" ), xml::attribute< std::string >( xis, "name" ).c_str() )
    , converter_( converter )
    , type_( types.Get( xml::attribute< std::string >( xis, "type" ) ) )
{
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
    if( Entity_ABC* superior = const_cast< Entity_ABC* >( Get< kernel::TacticalHierarchies >().GetSuperior() ) )
        static_cast< Team* >( superior )->Resolver< kernel::Object_ABC >::Remove( id_ ); // $$$$ SBO 2006-10-20: 
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
    displayer.Item( "" ).Start( Styles::bold ).Add( *(Object_ABC*)this ).End();
}

// -----------------------------------------------------------------------------
// Name: Object::SerializeAttributes
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void Object::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "type", type_.GetType() )
        << xml::attribute( "name", name_ );
}

// -----------------------------------------------------------------------------
// Name: Object::CreateDictionary
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void Object::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dico = *new PropertiesDictionary( controller );
    Attach( dico );
    const Object& constSelf = *this;
    dico.Register( *(const Entity_ABC*)this, tools::translate( "Object", "Info/Identifier" ), constSelf.id_ );
    dico.Register( *(const Entity_ABC*)this, tools::translate( "Object", "Info/Name" ), name_ );
    dico.Register( *(const Entity_ABC*)this, tools::translate( "Object", "Info/Type" ), constSelf.type_.GetType() );
}
