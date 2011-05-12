// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ObjectAttributeContainer.h"
#include "BypassAttribute.h"
#include "ConstructionAttribute.h"
#include "CrossingSiteAttribute.h"
#include "DelayAttribute.h"
#include "FireAttribute.h"
#include "BurnAttribute.h"
#include "FloodAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "LodgingAttribute.h"
#include "LogisticAttribute.h"
#include "KnowledgeGroup.h"
#include "MedicalTreatmentAttribute.h"
#include "MineAttribute.h"
#include "Model.h"
#include "NBCAttribute.h"
#include "ObjectKnowledge.h"
#include "ObstacleAttribute.h"
#include "SupplyRouteAttribute.h"
#include "ActivityTimeAttribute.h"
#include "ToxicCloudAttribute.h"
#include "InteractionHeightAttribute.h"
#include "StockAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "NBCTypeAttribute.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectAttributeContainer constructor
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
ObjectAttributeContainer::ObjectAttributeContainer( const Model_ABC& model )
    : model_( model )
{
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeContainer destructor
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
ObjectAttributeContainer::~ObjectAttributeContainer()
{
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CreateOrUpdate
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
template< typename T >
void ObjectAttributeContainer::CreateOrUpdate( const sword::ObjectAttributes& message )
{
    for( T_ObjectAttributes::iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        if( dynamic_cast< T* >( &*it ) )
        {
            it->Update( message );
            return;
        }
    attributes_.push_back( new T( message ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CreateOrUpdate
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
template< typename T >
void ObjectAttributeContainer::CreateOrUpdate( const sword::ObjectAttributes& message, const Model_ABC& model )
{
    for( T_ObjectAttributes::iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        if( dynamic_cast< T* >( &*it ) )
        {
            it->Update( message );
            return;
        }
    attributes_.push_back( new T( model, message ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeContainer destructor
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
void ObjectAttributeContainer::Update( const sword::ObjectAttributes& message )
{
    if( message.has_construction() )
        CreateOrUpdate< ConstructionAttribute >( message );
    if( message.has_obstacle() )
        CreateOrUpdate< ObstacleAttribute >( message );
    if( message.has_mine() )
        CreateOrUpdate< MineAttribute >( message );
    if( message.has_logistic() )
        CreateOrUpdate< LogisticAttribute >( message, model_ );
    if( message.has_lodging() )
        CreateOrUpdate< LodgingAttribute >( message, model_ );
    if( message.has_bypass() )
        CreateOrUpdate< BypassAttribute >( message );
    if( message.has_crossing_site() )
        CreateOrUpdate< CrossingSiteAttribute >( message );
    if( message.has_supply_route() )
        CreateOrUpdate< SupplyRouteAttribute >( message );
    if( message.has_nbc() )
        CreateOrUpdate< NBCAttribute >( message );
    if( message.has_fire() )
        CreateOrUpdate< FireAttribute >( message );
    if( message.has_burn() )
        CreateOrUpdate< BurnAttribute >( message );
    if( message.has_burn_surface() )
        CreateOrUpdate< BurnSurfaceAttribute >( message );
    if( message.has_medical_treatment() )
        CreateOrUpdate< MedicalTreatmentAttribute >( message );
    if( message.has_effect_delay() )
        CreateOrUpdate< DelayAttribute >( message );
    if( message.has_activity_time() )
        CreateOrUpdate< ActivityTimeAttribute >( message );
    if( message.has_toxic_cloud() )
        CreateOrUpdate< ToxicCloudAttribute >( message );
    if( message.has_interaction_height() )
        CreateOrUpdate< InteractionHeightAttribute >( message );
    if( message.has_stock() )
        CreateOrUpdate< StockAttribute >( message );
    if( message.has_resource_networks() )
        CreateOrUpdate< ResourceNetworkAttribute >( message );
    if( message.has_nbc_agent() )
        CreateOrUpdate< NBCTypeAttribute >( message );
    if( message.has_flood() )
        CreateOrUpdate< FloodAttribute >( message );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributeContainer destructor
// Created: NLD 2010-10-27
// -----------------------------------------------------------------------------
void ObjectAttributeContainer::Send( sword::ObjectAttributes& message ) const
{
    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( message ) ) );
}
