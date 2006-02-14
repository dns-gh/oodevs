// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledgeAttributes.h"
#include "App.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAttributes constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeAttributes::ObjectKnowledgeAttributes( Controller& controller, const Resolver_ABC< Object_ABC >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , pRealObject_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAttributes destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeAttributes::~ObjectKnowledgeAttributes()
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledgeAttributes::Update( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.oid_objet_reelPresent )
        pRealObject_ = resolver_.Find( message.oid_objet_reel );

    if( message.m.pertinencePresent )
        nRelevance_ = message.pertinence;

    if( message.m.localisationPresent )
    {
        points_.clear();
        points_.reserve( message.localisation.vecteur_point.n );
        for( uint i = 0; i < message.localisation.vecteur_point.n; ++i )
        {
            MT_Vector2D vPos;
            App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)message.localisation.vecteur_point.elem[i].data, vPos );    
            points_.push_back( vPos );
        }
    }

    if( message.m.pourcentage_constructionPresent )
        nPourcentageConstruction_ = message.pourcentage_construction;
    
    if( message.m.pourcentage_valorisationPresent )
        nPourcentageValorisation_ = message.pourcentage_valorisation;
    
    if( message.m.pourcentage_contournementPresent )
        nPourcentageContournement_ = message.pourcentage_contournement;
    
    if( message.m.en_preparationPresent )
        bEnPreparation_ = message.en_preparation;
    
    if( message.m.est_percuPresent )
        bIsPerceived_ = message.est_percu;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeAttributes::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void ObjectKnowledgeAttributes::Update( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    pRealObject_ = resolver_.Find( message.oid_objet_reel );
}

