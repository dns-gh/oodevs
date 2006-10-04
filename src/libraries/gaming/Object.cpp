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
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Team_ABC >& teamResolver, const Resolver_ABC< ObjectType >& typeResolver, const Resolver_ABC< DotationType >& dotationResolver )
    : controller_                    ( controller )
    , converter_                     ( converter )
    , type_                          ( typeResolver.Get( message.type ) )
    , nId_                           ( message.oid )
    , strName_                       ( message.nom )
    , team_                          ( teamResolver.Get( message.camp ) )
    , rConstructionPercentage_       ( 0.0 )
    , rValorizationPercentage_       ( 0.0 )
    , rBypassConstructionPercentage_ ( 0.0 )
    , bPrepared_                     ( false )
    , nTypeLocalisation_             ( message.localisation.type )
    , construction_                  ( 0 )
    , valorization_                  ( 0 )
    , nDotationConstruction_         ( 0 )
    , nDotationValorization_         ( 0 )
{
    RegisterSelf( *this );

    if( message.m.type_dotation_constructionPresent )
        construction_ = & dotationResolver.Get( message.type_dotation_construction );
    
    if( message.m.type_dotation_valorisationPresent )
        valorization_ = & dotationResolver.Get( message.type_dotation_valorisation );

    type_.manager_.LockIdentifier( nId_ );
}

// -----------------------------------------------------------------------------
// Name: Object::~Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::~Object()
{
    controller_.Delete( *(Object_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Object::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void Object::DoUpdate( const  kernel::InstanciationComplete& )
{
    controller_.Create( *(Object_ABC*)this );
}   

// -----------------------------------------------------------------------------
// Name: Object::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long Object::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
QString Object::GetName() const
{
    return QString( "%1 [%2]" ).arg( strName_ ).arg( nId_ );
}

// -----------------------------------------------------------------------------
// Name: Object::GetTeam
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const Team_ABC& Object::GetTeam() const
{
    return team_;
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
// Name: Object::DoUpdate
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object::DoUpdate( const ASN1T_MsgObjectUpdate& message )
{
    bPrepared_ = message.en_preparation;

    if( message.m.nb_dotation_constructionPresent )
        nDotationConstruction_ = message.nb_dotation_construction;
    if( message.m.nb_dotation_valorisationPresent )
        nDotationValorization_ = message.nb_dotation_valorisation;

    if( message.m.pourcentage_constructionPresent )
        rConstructionPercentage_ = message.pourcentage_construction;
    if( message.m.pourcentage_valorisationPresent )
        rValorizationPercentage_ = message.pourcentage_valorisation;
    if( message.m.pourcentage_creation_contournementPresent )
        rBypassConstructionPercentage_ = message.pourcentage_creation_contournement;

    controller_.Update( *(Object_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Object::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void Object::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Informations" ) )
             .Display( tools::translate( "Object", "Id:" ), nId_ )
             .Display( tools::translate( "Object", "Nom:" ), strName_ )
             .Display( tools::translate( "Object", "Type:" ), type_.GetName() )
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
