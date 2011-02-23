// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "NodeProperties.h"
#include "ResourceNetworkModel.h"
#include "ResourceTools_ABC.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>
#include <urban/ResourceNetworkAttribute.h>
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties()
    : functionalState_   ( 1.f )
    , oldFunctionalState_( 1.f )
    , tools_             ( 0 )
    , needUpdate_        ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( const ResourceTools_ABC& tools )
    : functionalState_   ( 1.f )
    , oldFunctionalState_( 1.f )
    , tools_             ( &tools )
    , needUpdate_        ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( xml::xistream& xis, const ResourceTools_ABC& tools )
    : functionalState_   ( 1.f )
    , oldFunctionalState_( 1.f )
    , tools_             ( &tools )
    , needUpdate_        ( false )
{
    Update( xis );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::NodeProperties( const NodeProperties& from )
    : functionalState_   ( 1.f )
    , oldFunctionalState_( 1.f )
    , tools_             ( from.tools_ )
    , needUpdate_        ( false )
{
    for( CIT_Elements it = from.elements_.begin(); it != from.elements_.end(); ++it )
        Register( it->first, *new NodeElement( *it->second ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties destructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeProperties::~NodeProperties()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Initialize
// Created: JSR 2011-02-23
// -----------------------------------------------------------------------------
void NodeProperties::Initialize( const urban::ResourceNetworkAttribute& urbanAttribute )
{
    const urban::ResourceNetworkAttribute::T_ResourceNodes& nodes = urbanAttribute.GetResourceNodes();
    for( urban::ResourceNetworkAttribute::CIT_ResourceNodes it = nodes.begin(); it != nodes.end(); ++it )
    {
        unsigned long resourceId = tools_->GetResourceId( it->second.resource_ );
        Register( resourceId, *new NodeElement( it->second, resourceId ) );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::SetTools
// Created: JSR 2010-11-17
// -----------------------------------------------------------------------------
void NodeProperties::SetTools( const ResourceTools_ABC& tools )
{
    tools_ = &tools;
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Update( xml::xistream& xis )
{
    xis >> xml::list( "node", *this, &NodeProperties::ReadNode );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::UpdateState
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::UpdateState( const ResourceNetworkModel& model )
{
    if( Count() == 0 )
        return;
    // update intermediate stocks
    Apply( boost::bind( &NodeElement::UpdateImmediateStock, _1, functionalState_ ) );
    // apply consumptions
    functionalState_ = 1.f;
    Apply( boost::bind( &NodeElement::Consume, _1, boost::ref( functionalState_ ) ) );
    Apply( boost::bind( &NodeElement::DistributeResource, _1, functionalState_, boost::cref( model ) ) );
    if( std::abs( oldFunctionalState_ - functionalState_ ) >= 0.01 )
    {
        oldFunctionalState_ = functionalState_;
        needUpdate_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Finalize
// Created: JSR 2011-01-17
// -----------------------------------------------------------------------------
void NodeProperties::Finalize()
{
    if( Count() == 0 )
        return;
    Apply( boost::bind( &NodeElement::Finalize, _1, boost::cref( *tools_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeProperties::Push( int quantity, unsigned long resourceId )
{
    NodeElement* element = Find( resourceId );
    if( element )
        element->Push( quantity );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::SetModifier
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void NodeProperties::SetModifier( float modifier )
{
    if( Count() == 0 )
        return;
    Apply( boost::bind( &NodeElement::SetModifier, _1, modifier ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::AddConsumption
// Created: JSR 2011-02-01
// -----------------------------------------------------------------------------
void NodeProperties::AddConsumption( unsigned long resourceId, double consumption )
{
    NodeElement* element = Find( resourceId );
    if( element )
        element->AddConsumption( consumption );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::NeedUpdate
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
bool NodeProperties::NeedUpdate() const
{
    if( Count() == 0 )
        return false;
    if( needUpdate_ )
        return true;
    tools::Iterator< const NodeElement& > it = CreateIterator();
    while( it.HasMoreElements() )
        if( it.NextElement().NeedUpdate() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void NodeProperties::Serialize( sword::UrbanAttributes_Infrastructures& msg ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Serialize( *msg.add_resource_network() );
    needUpdate_ = false;
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Serialize
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void NodeProperties::Serialize( sword::ObjectAttributeResourceNetwork& msg ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->Serialize( *msg.add_network() );
    needUpdate_ = false;
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void NodeProperties::Update( const google::protobuf::RepeatedPtrField< sword::MissionParameter_Value >& list )
{
    for( int i = 0; i< list.size(); ++i )
    {
        sword::MissionParameter_Value node = list.Get( i );
        unsigned int id = tools_->GetResourceId( node.list( 0 ).acharstr() );
        NodeElement* element = Find( id );
        if( element )
            element->Update( node );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::ReadNode
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void NodeProperties::ReadNode( xml::xistream& xis )
{
    std::string resourceName = xis.attribute< std::string >( "resource-type" );
    unsigned long resourceId = tools_->GetResourceId( resourceName );
    NodeElement* element = Find( resourceId );
    if( element )
        element->Update( xis );
    else
        Register( resourceId, *new NodeElement( xis, resourceId, resourceName ) );
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::GetFunctionalState
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
float NodeProperties::GetFunctionalState() const
{
    return functionalState_;
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::GetConsumptionState
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
float NodeProperties::GetConsumptionState( unsigned long resourceId ) const
{
    NodeElement* element = Find( resourceId );
    if( element )
        return element->GetConsumptionState();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: NodeProperties::AddToStock
// Created: BCI 2011-02-21
// -----------------------------------------------------------------------------
double NodeProperties::AddToStock( unsigned long resourceId, double quantity )
{
    NodeElement* element = Find( resourceId );
    if( element )
        return element->AddToStock( quantity );
    return 0.;
}
