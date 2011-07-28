// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledge.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectIcons.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Viewport_ABC.h"
#include "Tools.h"
#include "statusicons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Entity_ABC& owner, const sword::ObjectKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter,
                                  const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::Resolver_ABC< kernel::ObjectType, std::string >& typeResolver )
    : EntityImplementation< ObjectKnowledge_ABC >( controller, message.knowledge().id(), "" )
    , converter_     ( converter )
    , owner_         ( owner )
    , objectResolver_( objectResolver )
    , type_          ( & typeResolver.Get( message.type().id() ) )
    , pRealObject_   ( objectResolver_.Find( message.object().id() ) )
    , pTeam_         ( 0 )
{
    RegisterSelf( *this );

    //$$ NLD - 2010-11-03 - Ce bloc sucks
    if( pRealObject_ )
    {
        const Hierarchies* hierarchies = pRealObject_->Retrieve< TacticalHierarchies >();
        if( ! hierarchies )
            hierarchies = pRealObject_->Retrieve< CommunicationHierarchies >();
        const Entity_ABC& tmp = hierarchies ? hierarchies->GetTop() : *pRealObject_;
        pTeam_ = dynamic_cast< const kernel::Team_ABC* >( &tmp );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DoUpdate
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
{
    if( message.has_object()  )
        pRealObject_ = objectResolver_.Find( message.object().id() );
    if( message.has_relevance() )
        nRelevance_ = message.relevance();
    if( message.has_perceived() )
        bIsPerceived_ = message.perceived();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Information" ) )
                .Display( tools::translate( "Object", "Identifier:" ), id_ )
                .Display( tools::translate( "Object", "Associated object:" ), pRealObject_ )
                .Display( tools::translate( "Object", "Type:" ), type_ )
                .Display( tools::translate( "Object", "Perceived:" ), bIsPerceived_ )
                .Display( tools::translate( "Object", "Relevance:" ), nRelevance_ );
    if( ! position_.empty() )
        displayer.Group( tools::translate( "Object", "Information" ) )
                 .Display( tools::translate( "Object", "Location:" ), position_ );

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    if( pRealObject_ )
        displayer.Display( tools::findTranslation( "ObjectKnowledgePanel", "Known objects" ), pRealObject_ );
    else
        displayer.Display( tools::findTranslation( "ObjectKnowledgePanel", "Known objects" ), id_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Object", "Type:" ), type_ )
             .Display( tools::translate( "Object", "Relevance:" ), nRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetName
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
QString ObjectKnowledge::GetName() const
{
    return pRealObject_ ? pRealObject_->GetName() : tools::translate( "Object", "Unknown object" );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetEntity
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const Object_ABC* ObjectKnowledge::GetEntity() const
{
    return pRealObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetTeam
// Created: NLD 2010-11-03
// -----------------------------------------------------------------------------
const Team_ABC* ObjectKnowledge::GetTeam() const
{
    return pTeam_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetOwner
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const Entity_ABC& ObjectKnowledge::GetOwner() const
{
    return owner_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetSymbol
// Created: SBO 2009-06-04
// -----------------------------------------------------------------------------
std::string ObjectKnowledge::GetSymbol() const
{
    return type_ ? type_->GetSymbol() : "";
}
