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
#include "clients_gui/Tools.h"
#include "clients_kernel/IDManager.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Team_ABC.h"
#include "xeumeuleu/xml.h"
#include "IdManager.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( Controller& controller, const CoordinateConverter_ABC& converter, const kernel::ObjectType& type, Team_ABC& team, IdManager& idManager )
    : EntityImplementation< Object_ABC >( controller, idManager.GetNextId(), "" )
    , converter_                     ( converter )
    , type_                          ( type )
    , team_                          ( team )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
    , nDotationConstruction_         ( 0 )
    , nDotationValorization_         ( 0 )
{
    RegisterSelf( *this );
    name_ = QString( "%1 [%2]" ).arg( type.GetName() ).arg( id_ );
//    if( message.m.type_dotation_constructionPresent )
//        construction_ = & dotationResolver.Get( message.type_dotation_construction );
//    
//    if( message.m.type_dotation_valorisationPresent )
//        valorization_ = & dotationResolver.Get( message.type_dotation_valorisation );

//    type_.manager_.LockIdentifier( nId_ );
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
    displayer.Group( tools::translate( "Object", "Informations" ) )
             .Display( tools::translate( "Object", "Id:" ), id_ )
             .Display( tools::translate( "Object", "Nom:" ), name_ )
             .Display( tools::translate( "Object", "Type:" ), type_ )
             .Display( tools::translate( "Object", "Position:" ), converter_.ConvertToMgrs( Get< Positions >().GetPosition() ) ) // $$$$ AGE 2006-03-22: 
             .Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Valorisation:" ), rValorizationPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Contournement:" ), rBypassConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "En préparation:" ), bPrepared_ )
             .Item( tools::translate( "Object", "Dotation construction:" ) )
                .Start( nDotationConstruction_ )
                .Add( " " ).Add( construction_ ).End(); // $$$$ AGE 2006-02-22: End devrait renvoyer le parent
    displayer.Group( tools::translate( "Object", "Informations" ) )
             .Item( tools::translate( "Object", "Dotation valorisation:" ) )
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
             .Display( tools::translate( "Object", "Niveau de construction" ), rConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Valorisation" ),           rValorizationPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Contournement" ),          rBypassConstructionPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: Object::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Object::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    type_.Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Object::DoSerialize
// Created: SBO 2006-09-12
// -----------------------------------------------------------------------------
void Object::DoSerialize( xml::xostream& xos ) const
{
    xos << attribute( "id", long( id_ ) )
        << attribute( "type", type_.GetName().ascii() )
        << attribute( "name", name_ );
}
