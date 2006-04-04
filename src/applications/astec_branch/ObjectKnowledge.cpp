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
#include "Displayer_ABC.h"
#include "Units.h"
#include "Object.h"
#include "ObjectType.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const ASN1T_MsgObjectKnowledgeCreation& message, Controller& controller, const CoordinateConverter& converter, 
                                  const Resolver_ABC< Object >& objectResolver , const Resolver_ABC< Agent >& agentResolver, const Resolver_ABC< ObjectType >& typeResolver )
    : converter_     ( converter )
    , objectResolver_( objectResolver )
    , agentResolver_ ( agentResolver )
    , controller_    ( controller )
    , id_            ( message.oid_connaissance ) 
    , type_          ( & typeResolver.Get( message.type ) )
    , pRealObject_   ( 0 )
{
    // $$$$ AGE 2006-02-14: Team !
    pRealObject_ = objectResolver_.Find( message.oid_objet_reel );
    InterfaceContainer< Extension_ABC >::Register( *this );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    controller_.Delete( *this );
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
            points_.push_back( converter_.ConvertToXY( message.localisation.vecteur_point.elem[i] ) );
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
Object* ObjectKnowledge::GetRealObject() const
{
    return pRealObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "D�tails" )
                .Display( "Id:", id_ )
                .Display( "Objet associ�:", pRealObject_ )
                .Display( "Type:", type_ )
                .Display( "Construction:", nPourcentageConstruction_ * Units::percentage )
                .Display( "Valeur:", nPourcentageValorisation_ * Units::percentage )
                .Display( "Contournement:", nPourcentageContournement_* Units::percentage )
                .Display( "En pr�paration:", bEnPreparation_ )
                .Display( "Per�u:", bIsPerceived_ )
                .Display( "Pertinence:", nRelevance_ );
    if( ! points_.empty() )
        displayer.Group( "D�tails" ).Display( "Position:", converter_.ConvertToMgrs( points_.front() ) );

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    if( pRealObject_ )
        displayer.Display( "Objets connus", pRealObject_ );
    else
        displayer.Display( "Objets connus", id_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::IsInTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
bool ObjectKnowledge::IsInTeam( const Team& team ) const
{
    return pRealObject_ && & pRealObject_->GetTeam() == &team;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetId
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
unsigned long ObjectKnowledge::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetName
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
std::string ObjectKnowledge::GetName() const
{
    return pRealObject_ ? pRealObject_->GetName() : "Unknown Object";
}
