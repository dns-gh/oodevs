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
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/ObjectIcons.h"
#include "Tools.h"
#include "statusicons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message, Controller& controller, const CoordinateConverter_ABC& converter, 
                                  const Resolver_ABC< Object_ABC >& objectResolver, const Resolver_ABC< ObjectType >& typeResolver )
    : EntityImplementation< ObjectKnowledge_ABC >( controller, message.oid, "" )
    , converter_     ( converter )
    , owner_         ( owner )
    , objectResolver_( objectResolver )
    , type_          ( & typeResolver.Get( message.type ) )
    , pRealObject_   ( 0 )
{
    RegisterSelf( *this );
    pRealObject_ = objectResolver_.Find( message.real_object );

    if( message.m.obstacle_typePresent )
        obstacleType_ = (E_ObstacleType)message.obstacle_type;   
    if( message.m.reserved_obstacle_activatedPresent )
        reservedObstacleActivated_ = message.reserved_obstacle_activated != 0;
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
void ObjectKnowledge::DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    if( message.m.real_objectPresent )
        pRealObject_ = objectResolver_.Find( message.real_object );

    if( message.m.relevancePresent )
        nRelevance_ = message.relevance;

    // $$$$ AGE 2008-04-03: 
//    if( message.m.locationPresent && message.location.coordinates.n )
//        position_ = std::string( (const char*)( message.location.coordinates.elem[0].data ), 15 );

    if( message.m.construction_percentagePresent )
        nConstructionPercentage_ = message.construction_percentage;
    
    if( message.m.mining_percentagePresent )
        nValorizationPercentage_ = message.mining_percentage;
    
    if( message.m.bypass_construction_percentagePresent )
        nBypassConstructionPercentage_ = message.bypass_construction_percentage;
    
    if( message.m.reserved_obstacle_activatedPresent )
        reservedObstacleActivated_ = message.reserved_obstacle_activated != 0;
    
    if( message.m.perceivedPresent )
        bIsPerceived_ = message.perceived != 0;

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
                .Display( tools::translate( "Object", "Construction:" ), nConstructionPercentage_ * Units::percentage )
                .Display( tools::translate( "Object", "Mining:" ), nValorizationPercentage_ * Units::percentage )
                .Display( tools::translate( "Object", "Bypass:" ), nBypassConstructionPercentage_* Units::percentage )
                .Display( tools::translate( "Object", "Obstacle type:" ), obstacleType_ )
                .Display( tools::translate( "Object", "Reserved obstacle activated:" ), reservedObstacleActivated_ )
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
             .Display( tools::translate( "Object", "Relevance:" ), nRelevance_ )
             .Display( tools::translate( "Object", "Construction:" ), nConstructionPercentage_ * Units::percentage );
    if( nValorizationPercentage_.IsSet() )
        displayer.Display( tools::translate( "Object", "Mining:" ), nValorizationPercentage_ * Units::percentage );
    if( nBypassConstructionPercentage_.IsSet() )
        displayer.Display( tools::translate( "Object", "Bypass:" ), nBypassConstructionPercentage_ * Units::percentage );
    if( obstacleType_.IsSet() )
        displayer.Display( tools::translate( "Object", "Obstacle type:" ), obstacleType_ );
    if( reservedObstacleActivated_.IsSet() )
        displayer.Display( tools::translate( "Object", "Reserved obstacle activated:" ), reservedObstacleActivated_ );
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
const Team_ABC& ObjectKnowledge::GetOwner() const
{
    return owner_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Draw
// Created: AGE 2006-05-19
// -----------------------------------------------------------------------------
void ObjectKnowledge::Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( type_ )
        ObjectIcons::Draw( type_->GetId(), where, viewport, tools );
    if( viewport.IsVisible( where ) )
    {
        // $$$$ SBO 2007-05-04: hard coded icon positions
        glPushAttrib( GL_CURRENT_BIT );
            glColor3f( 1, 1, 1 );
            if( reservedObstacleActivated_.IsSet() )
                tools.DrawIcon( reservedObstacleActivated_ ? xpm_activated : xpm_not_activated, where + geometry::Vector2f( 250.f, 150.f ), 150.f );
            if( nConstructionPercentage_.IsSet() )
                tools.DrawLife( where - geometry::Vector2f( 0.f, 250.f ), nConstructionPercentage_ / 100.f );
            if( nBypassConstructionPercentage_.IsSet() )
                tools.DrawLife( where - geometry::Vector2f( 0.f, 200.f ), nBypassConstructionPercentage_ / 100.f );
        glPopAttrib();
    }
}
