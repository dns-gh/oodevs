//*****************************************************************************
//
// $Created: AGN 03-07-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectKnowledge.cpp $
// $Author: Ape $
// $Modtime: 4/11/04 11:15 $
// $Revision: 3 $
// $Workfile: ObjectKnowledge.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledge.h"
#include "Controller.h"
#include "App.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& message, Controller& controller , const Resolver_ABC< Object_ABC >& objectResolver , const Resolver_ABC< Agent >& agentResolver )
    : objectResolver_( objectResolver )
    , agentResolver_( agentResolver )
    , controller_( controller )
    , id_        ( message.oid_connaissance ) 
    , type_      ( message.type )
    , pRealObject_( 0 )
{
    // $$$$ AGE 2006-02-14: Team !
    pRealObject_ = objectResolver_.Find( message.oid_objet_reel );
    InterfaceContainer< Extension_ABC >::Register( *this );
}


// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.oid_objet_reelPresent )
        pRealObject_ = objectResolver_.Find( message.oid_objet_reel );

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

    if( message.m.perception_par_compagniePresent )
    {
        detectingAutomats_.clear();
        for( uint i = 0; i < message.perception_par_compagnie.n; ++i )
            detectingAutomats_.insert( & agentResolver_.Get( message.perception_par_compagnie.elem[i] ) );
    }

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetRealObject
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Object_ABC* ObjectKnowledge::GetRealObject() const
{
    return pRealObject_;
}
