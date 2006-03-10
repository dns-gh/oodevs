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

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Object_ABC.h"

#include "Net_Def.h"
#include "App.h"
#include "World.h"
#include "Tools.h"
#include "FireResult.h"
#include "Controller.h"
#include "Units.h"
#include "Displayer_ABC.h"
#include "ObjectType.h"

// $$$$ AGE 2006-02-16: possession des objets par la team ?
// $$$$ AGE 2006-02-16: ou au moins les enregistrer ?

// -----------------------------------------------------------------------------
// Name: Object_ABC::Object_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::Object_ABC( const ASN1T_MsgObjectCreation& message, Controller& controller, const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< ObjectType >& typeResolver, const Resolver_ABC< DotationType >& dotationResolver )
    : controller_( controller )
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
        MT_Vector2D vTmp;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.localisation.vecteur_point.elem[i].data, vTmp );
        pointVector_.push_back( vTmp );
        center_ += vTmp;
    }

    if( ! pointVector_.empty() )
        center_ /= pointVector_.size();

    if( message.m.type_dotation_constructionPresent )
        construction_ = & dotationResolver.Get( message.type_dotation_construction );
    
    if( message.m.type_dotation_valorisationPresent )
        valorization_ = & dotationResolver.Get( message.type_dotation_valorisation );

    controller_.Create( *this );
}



// -----------------------------------------------------------------------------
// Name: Object_ABC::~Object_ABC
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
Object_ABC::~Object_ABC()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long Object_ABC::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetName
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
std::string Object_ABC::GetName() const
{
    std::stringstream stream;
    stream << strName_ << " [" << nId_ << ']';
    return stream.str();
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetTeam
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
const Team& Object_ABC::GetTeam() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::GetType
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
ObjectType& Object_ABC::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object_ABC::DoUpdate
// Created: SBO 2005-09-02
// -----------------------------------------------------------------------------
void Object_ABC::DoUpdate( const ASN1T_MsgObjectUpdate& message )
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
        center_.Reset();
        pointVector_.clear();
        pointVector_.reserve( message.localisation.vecteur_point.n );
        nTypeLocalisation_ = message.localisation.type;
        for( uint i = 0; i < message.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vTmp;
            App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.localisation.vecteur_point.elem[i].data, vTmp );
            pointVector_.push_back( vTmp );
            center_ += vTmp;
        }
        if( pointVector_.size() > 1 )
            center_ /= pointVector_.size();
    }
    controller_.Update( *this );
}


// -----------------------------------------------------------------------------
// Name: Object_ABC::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void Object_ABC::Display( Displayer_ABC& displayer ) const
{
    std::string strPos;
    App::GetApp().GetWorld().SimToMosMgrsCoord( center_, strPos );

    displayer.Group( "Informations" )
             .Display( "Id:", nId_ )
             .Display( "Nom:", strName_ )
             .Display( "Type:", type_.GetName() )
             .Display( "Position:", strPos )
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

   



   