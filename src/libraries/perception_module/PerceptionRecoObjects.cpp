// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionRecoObjects.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "ListInCircleVisitor.h"
#include "wrapper/Hook.h"
#include "wrapper/Effect.h"
#include "wrapper/Event.h"
#include <boost/bind.hpp>

using namespace sword;
using namespace sword::perception;

DECLARE_HOOK( CanObjectBePerceived, bool, ( const MIL_Object_ABC* object ) )
DECLARE_HOOK( IsLocalizationInsideCircle, bool, ( const SWORD_Model* localization, const MT_Vector2D* center, double radius ) )
DECLARE_HOOK( IsKnowledgeObjectInsidePerception, bool, ( const SWORD_Model* localization, const MT_Vector2D* center, double radius, const DEC_Knowledge_Object* object ) )
DECLARE_HOOK( IsObjectIntersectingLocalization, bool, ( const SWORD_Model* localization, const MIL_Object_ABC* object ) )
DECLARE_HOOK( GetObjectListWithinCircle, void, ( const MT_Vector2D& vCenter, double rRadius, void (*callback)( MIL_Object_ABC* object, void* userData ), void* userData ) )

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjectsReco constructor
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
PerceptionRecoObjectsReco::PerceptionRecoObjectsReco( const wrapper::View& perception, const wrapper::View& entity )
    : vCenter_     ( perception[ "center/x" ], perception[ "center/y" ] )
    , localisation_( perception[ "localization" ] )
    , rCurrentSize_( static_cast< double >( perception[ "radius" ] ) + perception[ "growth-speed" ] )
{
    if( perception[ "max-radius-reached" ] )
        return;
    wrapper::Effect effect( perception[ "radius" ] );
    effect = rCurrentSize_;
    effect.Post();
    if( GET_HOOK( IsLocalizationInsideCircle )( localisation_, &vCenter_, rCurrentSize_ ) )
    {
        wrapper::Effect effect( perception[ "max-radius-reached" ] );
        effect = true;
        effect.Post();
    }
    wrapper::Event event( "perception callback" );
    event[ "entity" ] = static_cast< std::size_t >( entity[ "identifier" ] );
    event[ "perception" ] = static_cast< std::size_t >( perception[ "perception-id" ] );
    event.Post();
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjectsReco::IsInside
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
bool PerceptionRecoObjectsReco::IsInside( const DEC_Knowledge_Object& knowledge ) const
{
    return GET_HOOK( IsKnowledgeObjectInsidePerception )( localisation_, &vCenter_, rCurrentSize_, &knowledge );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjectsReco::GetObjectsInside
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PerceptionRecoObjectsReco::GetObjectsInside( const wrapper::View&, Perception_ABC::T_ObjectVector& result ) const
{
    ListInCircleVisitor< MIL_Object_ABC* > objectVisitor( result );
    GET_HOOK( GetObjectListWithinCircle )( vCenter_, rCurrentSize_, &ListInCircleVisitor< MIL_Object_ABC* >::Add, &objectVisitor );
    for( Perception_ABC::T_ObjectVector::iterator it = result.begin(); it != result.end(); )
        if( GET_HOOK( IsObjectIntersectingLocalization )( localisation_, *it ) )
            ++it;
        else
            it = result.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjects::PerceptionRecoObjects
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoObjects::PerceptionRecoObjects( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : observer_( observer )
{
    entity[ "perceptions/object-detection" ].VisitIdentifiedChildren( boost::bind( &PerceptionRecoObjects::AddLocalisation, this, _2, boost::cref( entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjects::~PerceptionRecoObjects
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
PerceptionRecoObjects::~PerceptionRecoObjects()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjects::AddLocalisation
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void PerceptionRecoObjects::AddLocalisation( const wrapper::View& perception, const wrapper::View& entity )
{
    Add( std::auto_ptr< PerceptionRecoObjectsReco >( new PerceptionRecoObjectsReco( perception, entity ) ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjects::Compute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionRecoObjects::Compute( const wrapper::View& /*perceiver*/, const SurfacesObject_ABC& /*surfaces*/, const DEC_Knowledge_Object& knowledge ) const
{
    for( T_RecoVector::const_iterator it = recos_.begin(); it != recos_.end(); ++it )
        if( (*it)->IsInside( knowledge ) )
            return PerceptionLevel::recognized_;
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionRecoObjects::Execute
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void PerceptionRecoObjects::Execute( const wrapper::View& perceiver, const SurfacesObject_ABC& /*surfaces*/, const T_ObjectVector& /*perceivableObjects*/ )
{
    for( T_RecoVector::const_iterator itReco = recos_.begin(); itReco != recos_.end(); ++itReco )
    {
        T_ObjectVector perceivableObjects;
        (*itReco)->GetObjectsInside( perceiver, perceivableObjects );
        for( T_ObjectVector::const_iterator it = perceivableObjects.begin(); it != perceivableObjects.end(); ++it )
        {
            MIL_Object_ABC* object = *it;
            if( GET_HOOK( CanObjectBePerceived )( object ) )
                observer_.NotifyPerception( object, PerceptionLevel::identified_ ); // Identifié ou not seen pour les objets
        }
    }
}
