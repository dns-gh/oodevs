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
#include "clients_gui/Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( Controller& controller, const CoordinateConverter_ABC& converter, const kernel::ObjectType& type, const QString& name, bool prepare, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, idManager.GetNextId(), "" )
    , converter_                     ( converter )
    , type_                          ( type )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( prepare )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
    , nDotationConstruction_         ( 0 )
    , nDotationValorization_         ( 0 )
{
    RegisterSelf( *this );
    name_ = name.isEmpty() ? tools::translate( "Object", "%1 [%2]" ).arg( type.GetName() ).arg( id_ ) : name;
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
Object::Object( xml::xistream& xis, kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType, QString >& types, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, ReadId( xis ), ReadName( xis ) )
    , converter_                     ( converter )
    , type_                          ( ReadType( xis, types ) )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( ReadPrepared( xis ) )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
    , nDotationConstruction_         ( 0 )
    , nDotationValorization_         ( 0 )
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
// Name: Object::ReadId
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
unsigned long Object::ReadId( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    return (unsigned long)id;
}

// -----------------------------------------------------------------------------
// Name: Object::ReadName
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
QString Object::ReadName( xml::xistream& xis )
{
    std::string name;
    xis >> attribute( "name", name );
    return name.c_str();
}

// -----------------------------------------------------------------------------
// Name: Object::ReadType
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
const kernel::ObjectType& Object::ReadType( xml::xistream& xis, const Resolver_ABC< ObjectType, QString >& types )
{
    std::string type;
    xis >> attribute( "type", type );
    return types.Get( QString( type.c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: Object::ReadPrepared
// Created: SBO 2007-03-20
// -----------------------------------------------------------------------------
bool Object::ReadPrepared( xml::xistream& xis )
{
    bool prepared = false;
    xis >> xml::optional() >> attribute( "prepared", prepared );
    return prepared;
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
             .Display( tools::translate( "Object", "Location:" ), converter_.ConvertToMgrs( Get< Positions >().GetPosition() ) ) // $$$$ AGE 2006-03-22: 
             .Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Development:" ), rValorizationPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Bypass:" ), rBypassConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Setting up:" ), bPrepared_ )
             .Item( tools::translate( "Object", "Construction dotation:" ) )
                .Start( nDotationConstruction_ )
                .Add( " " ).Add( construction_ ).End(); // $$$$ AGE 2006-02-22: End devrait renvoyer le parent
    displayer.Group( tools::translate( "Object", "Information" ) )
             .Item( tools::translate( "Object", "Development dotation:" ) )
                .Start( nDotationValorization_ )
                .Add( " " ).Add( valorization_ ).End();
}

// -----------------------------------------------------------------------------
// Name: Object::DisplayInTooltip
// Created: AGE 2006-06-29
// -----------------------------------------------------------------------------
void Object::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    displayer.Display( "", *(Object_ABC*)this )
             .Display( tools::translate( "Object", "Construction:" ),  rConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Development:" ),   rValorizationPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Bypass:" ),        rBypassConstructionPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: Object::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Object::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    type_.Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Object::SerializeAttributes
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void Object::SerializeAttributes( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_.GetName().ascii() )
        << attribute( "name", name_ )
        << attribute( "prepared", bPrepared_ );
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
    dico.Register( *(const Entity_ABC*)this, tools::translate( "Object", "Info/Type" ), constSelf.type_ );
    dico.Register( *(const Entity_ABC*)this, tools::translate( "Object", "Info/Prepare" ), bPrepared_ );
}
