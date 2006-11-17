// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Object.h"

#include "Net_Def.h"
#include "Tools.h"
#include "clients_kernel/IDManager.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

const QString Object::typeName_ = "object";

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< ObjectType >& typeResolver, const Resolver_ABC< DotationType >& dotationResolver )
    : EntityImplementation< Object_ABC >( controller, message.oid, QString( "%1 [%2]" ).arg( message.nom ).arg( message.oid ) )
    , converter_                     ( converter )
    , type_                          ( typeResolver.Get( message.type ) )
    , nTypeLocalisation_             ( message.localisation.type )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
{
    RegisterSelf( *this );

    if( message.m.type_dotation_constructionPresent )
        construction_ = & dotationResolver.Get( message.type_dotation_construction );
    
    if( message.m.type_dotation_valorisationPresent )
        valorization_ = & dotationResolver.Get( message.type_dotation_valorisation );

    type_.manager_.LockIdentifier( id_ );
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
ObjectType& Object::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString Object::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: Object::DoUpdate
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
     // $$$$ AGE 2006-10-17: toooo bad. NLD sux !
    if( type_.CanBePrepared() )
        bPrepared_ = message.en_preparation;

    if( message.m.nb_dotation_constructionPresent )
        nDotationConstruction_ = message.nb_dotation_construction;
    if( message.m.nb_dotation_valorisationPresent && type_.CanBeValorized() )
        nDotationValorization_ = message.nb_dotation_valorisation;

    if( message.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = message.pourcentage_construction;
    if( message.m.pourcentage_valorisationPresent && type_.CanBeValorized() )
        rValorizationPercentage_ = message.pourcentage_valorisation;
    if( message.m.pourcentage_creation_contournementPresent && type_.CanBeBypassed() )
        rBypassConstructionPercentage_ = message.pourcentage_creation_contournement;

    Touch();
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
                .Add( " " ).Add( construction_ ).End(); // $$$ AGE 2006-02-22: End devrait renvoyer le parent
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
             .Display( tools::translate( "Object", "Construction:" ), rConstructionPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Valorisation:" ),           rValorizationPercentage_ * Units::percentage )
             .Display( tools::translate( "Object", "Contournement:" ),          rBypassConstructionPercentage_ * Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: Object::Draw
// Created: AGE 2006-03-24
// -----------------------------------------------------------------------------
void Object::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    type_.Draw( where, viewport, tools );
}
