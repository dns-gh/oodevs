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
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectIcons.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/TacticalHierarchies.h"
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
ObjectKnowledge::ObjectKnowledge( const Entity_ABC& owner, const MsgsSimToClient::MsgObjectKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, 
                                  const tools::Resolver_ABC< Object_ABC >& objectResolver, const tools::Resolver_ABC< ObjectType, std::string >& typeResolver )
    : EntityImplementation< ObjectKnowledge_ABC >( controller, message.oid(), "" )
    , converter_     ( converter )
    , owner_         ( owner )
    , objectResolver_( objectResolver )
    , type_          ( & typeResolver.Get( message.type() ) )
    , pRealObject_   ( 0 )
{
    RegisterSelf( *this );
    pRealObject_ = objectResolver_.Find( message.real_object() );
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
void ObjectKnowledge::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    if( message.has_real_object()  )
        pRealObject_ = objectResolver_.Find( message.real_object() );

    if( message.has_relevance()  )
        nRelevance_ = message.relevance();

    Touch();
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Object", "Details" ) )
                .Display( tools::translate( "Object", "Identifier:" ), id_ )
                .Display( tools::translate( "Object", "Associated object:" ), pRealObject_ )
                .Display( tools::translate( "Object", "Type:" ), type_ )
                .Display( tools::translate( "Object", "Perceived:" ), bIsPerceived_ )
                .Display( tools::translate( "Object", "Relevance:" ), nRelevance_ );
    if( ! position_.empty() )
        displayer.Group( tools::translate( "Object", "Details" ) )
                 .Display( tools::translate( "Object", "Location:" ), position_ );

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    if( pRealObject_ )
        displayer.Display( tools::translate( "Object", "Known objects" ), pRealObject_ );
    else
        displayer.Display( tools::translate( "Object", "Known objects" ), id_ );
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
// Name: ObjectKnowledge::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString ObjectKnowledge::GetTypeName() const
{
    return typeName_;
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
// Name: ObjectKnowledge::GetRecognizedEntity
// Created: SBO 2006-12-08
// -----------------------------------------------------------------------------
const Entity_ABC* ObjectKnowledge::GetRecognizedEntity() const
{
    return pRealObject_;
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

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
