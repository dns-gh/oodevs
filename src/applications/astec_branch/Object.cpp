// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "astec_pch.h"
#include "Object.h"

#include "Net_Def.h"
#include "Tools.h"
#include "Controller.h"
#include "Units.h"
#include "Displayer_ABC.h"
#include "ObjectType.h"
#include "CoordinateConverter.h"

// $$$$ AGE 2006-02-16: possession des objets par la team ?
// $$$$ AGE 2006-02-16: ou au moins les enregistrer ?

// -----------------------------------------------------------------------------
// Name: Object::Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::Object( const ASN1T_MsgObjectCreation& message, Controller& controller, const CoordinateConverter& converter, const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< ObjectType >& typeResolver, const Resolver_ABC< DotationType >& dotationResolver )
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
    InterfaceContainer< Extension_ABC >::Register( *this );

    for( uint i = 0; i < message.localisation.vecteur_point.n; ++i )
    {
        pointVector_.push_back( converter_.ConvertToXY( message.localisation.vecteur_point.elem[i] ) );
        center_ += geometry::Vector2f( pointVector_.back().X(), pointVector_.back().Y() );
    }

    if( ! pointVector_.empty() )
        center_.Set( center_.X() / pointVector_.size(), center_.Y() / pointVector_.size() );

    if( message.m.type_dotation_constructionPresent )
        construction_ = & dotationResolver.Get( message.type_dotation_construction );
    
    if( message.m.type_dotation_valorisationPresent )
        valorization_ = & dotationResolver.Get( message.type_dotation_valorisation );

    controller_.Create( *this );
}



// -----------------------------------------------------------------------------
// Name: Object::~Object
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object::~Object()
{
    controller_.Delete( *this );
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
std::string Object::GetName() const
{
    std::stringstream stream;
    stream << strName_ << " [" << nId_ << ']';
    return stream.str();
}

// -----------------------------------------------------------------------------
// Name: Object::GetTeam
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const Team& Object::GetTeam() const
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

    if( message.m.localisationPresent )
    {
        center_.Set( 0, 0 );
        pointVector_.clear();
        pointVector_.reserve( message.localisation.vecteur_point.n );
        nTypeLocalisation_ = message.localisation.type;
        for( uint i = 0; i < message.localisation.vecteur_point.n; ++i )
        {
            pointVector_.push_back( converter_.ConvertToXY( message.localisation.vecteur_point.elem[i] ) );
            center_ += geometry::Vector2f( pointVector_.back().X(), pointVector_.back().Y() );
        }
        if( pointVector_.size() > 1 )
            center_.Set( center_.X() / pointVector_.size(), center_.Y() / pointVector_.size() );
    }
    controller_.Update( *this );
}


// -----------------------------------------------------------------------------
// Name: Object::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void Object::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Informations" )
             .Display( "Id:", nId_ )
             .Display( "Nom:", strName_ )
             .Display( "Type:", type_.GetName() )
             .Display( "Position:", converter_.ConvertToMgrs( center_ ) )
             .Display( "Construction:", rConstructionPercentage_ * Units::percentage )
             .Display( "Valorisation:", rValorizationPercentage_ * Units::percentage )
             .Display( "Contournement:", rBypassConstructionPercentage_ * Units::percentage )
             .Display( "En préparation:", bPrepared_ )
             .Item( "Dotation construction:" )
                .Start( nDotationConstruction_ )
                .Add( " " ).Add( construction_ ).End(); // $$$$ AGE 2006-02-22: End devrait renvoyer le parent
    displayer.Group( "Informations" )
             .Item( "Dotation valorisation:" )
                .Start( nDotationValorization_ )
                .Add( " " ).Add( valorization_ ).End();

}

   



   