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
#include "CoordinateConverter_ABC.h"
#include "Team.h"
#include "GlTools_ABC.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Team& owner, const ASN1T_MsgObjectKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, 
                                  const Resolver_ABC< Object >& objectResolver , const Resolver_ABC< Agent >& agentResolver, const Resolver_ABC< ObjectType >& typeResolver )
    : owner_         ( owner )
    , converter_     ( converter )
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

    if( message.m.localisationPresent && message.localisation.vecteur_point.n )
        position_ = std::string( (const char*)( message.localisation.vecteur_point.elem[0].data ), 15 );

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
// Name: ObjectKnowledge::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Détails" )
                .Display( "Id:", id_ )
                .Display( "Objet associé:", pRealObject_ )
                .Display( "Type:", type_ )
                .Display( "Construction:", nPourcentageConstruction_ * Units::percentage )
                .Display( "Valeur:", nPourcentageValorisation_ * Units::percentage )
                .Display( "Contournement:", nPourcentageContournement_* Units::percentage )
                .Display( "En préparation:", bEnPreparation_ )
                .Display( "Perçu:", bIsPerceived_ )
                .Display( "Pertinence:", nRelevance_ );
    if( ! position_.empty() )
        displayer.Group( "Détails" ).Display( "Position:", position_ );

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
    return owner_ == team;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::KnowledgeIsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool ObjectKnowledge::KnowledgeIsInTeam( const Team& team ) const
{
    return pRealObject_ && pRealObject_->GetTeam() == team;
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

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetKnowledgeTeam
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const Team* ObjectKnowledge::GetKnowledgeTeam() const
{
    return pRealObject_ ? & pRealObject_->GetTeam() : 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Draw
// Created: AGE 2006-05-19
// -----------------------------------------------------------------------------
void ObjectKnowledge::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( bIsPerceived_ && pRealObject_ )
    {
        const bool backupState = tools.Select( false );
        pRealObject_->Entity_ABC::Draw( where, viewport, tools );
        tools.Select( backupState );
    }
}
