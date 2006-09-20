// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledge.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, 
                                  const Resolver_ABC< Object_ABC >& objectResolver , const Resolver_ABC< Agent_ABC >& agentResolver, const Resolver_ABC< ObjectType >& typeResolver )
    : owner_         ( owner )
    , converter_     ( converter )
    , objectResolver_( objectResolver )
    , agentResolver_ ( agentResolver )
    , controller_    ( controller )
    , id_            ( message.oid_connaissance ) 
    , type_          ( & typeResolver.Get( message.type ) )
    , pRealObject_   ( 0 )
{
    RegisterSelf( *this );
    pRealObject_ = objectResolver_.Find( message.oid_objet_reel );
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
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const kernel::InstanciationComplete& )
{
    controller_.Create( *this );
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
    displayer.Group( tools::translate( "Object", "D�tails" ) )
                .Display( tools::translate( "Object", "Id:" ), id_ )
                .Display( tools::translate( "Object", "Objet associ�:" ), pRealObject_ )
                .Display( tools::translate( "Object", "Type:" ), type_ )
                .Display( tools::translate( "Object", "Construction:" ), nPourcentageConstruction_ * Units::percentage )
                .Display( tools::translate( "Object", "Valeur:" ), nPourcentageValorisation_ * Units::percentage )
                .Display( tools::translate( "Object", "Contournement:" ), nPourcentageContournement_* Units::percentage )
                .Display( tools::translate( "Object", "En pr�paration:" ), bEnPreparation_ )
                .Display( tools::translate( "Object", "Per�u:" ), bIsPerceived_ )
                .Display( tools::translate( "Object", "Pertinence:" ), nRelevance_ );
    if( ! position_.empty() )
        displayer.Group( tools::translate( "Object", "D�tails" ) )
                 .Display( tools::translate( "Object", "Position:" ), position_ );

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    if( pRealObject_ )
        displayer.Display( tools::translate( "Object", "Objets connus" ), pRealObject_ );
    else
        displayer.Display( tools::translate( "Object", "Objets connus" ), id_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::IsInTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
bool ObjectKnowledge::IsInTeam( const Team_ABC& team ) const
{
    return & owner_ == & team;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::KnowledgeIsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool ObjectKnowledge::KnowledgeIsInTeam( const Team_ABC& team ) const
{
    return pRealObject_ && & pRealObject_->GetTeam() == & team;
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
QString ObjectKnowledge::GetName() const
{
    // $$$$ AGE 2006-08-23: 
    return pRealObject_ ? pRealObject_->GetName() : "Unknown Object";
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetKnowledgeTeam
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const Team_ABC* ObjectKnowledge::GetKnowledgeTeam() const
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

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Select
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void ObjectKnowledge::Select( ActionController& controller ) const
{
    controller.Select( *this );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::ContextMenu
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void ObjectKnowledge::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    controller.ContextMenu( *this, where );
}
    
// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Activate
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void ObjectKnowledge::Activate( ActionController& controller ) const
{
    controller.Activate( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetRealObject
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const kernel::Object_ABC* ObjectKnowledge::GetRealObject() const
{
    return pRealObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetTeam
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const kernel::Team_ABC& ObjectKnowledge::GetTeam() const
{
    return owner_;
}
