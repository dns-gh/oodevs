// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "NodeElement.h"
#include "ResourceLink.h"
#include "ResourceNetworkModel.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement()
    : model_              ( 0 )
    , resourceType_       ( static_cast< E_ResourceType>( -1 ) )
    , isActivated_        ( true )
    , productionCapacity_ ( 0 )
    , stockCapacity_      ( 0 )
    , stockMaxCapacity_   ( 0 )
    , immediateStock_     ( 0 )
    , receivedQuantity_   ( 0 )
    , consumptionAmount_  ( 0 )
    , consumptionCritical_( false )
    , modifier_           ( 1. ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement( xml::xistream& xis, E_ResourceType resourceType )
    : model_              ( 0 )
    , resourceType_       ( resourceType )
    , isActivated_        ( true )
    , productionCapacity_ ( xis.attribute< unsigned int >( "production" ) )
    , stockCapacity_      ( 0 )
    , stockMaxCapacity_   ( 0 )
    , immediateStock_     ( 0 )
    , receivedQuantity_   ( 0 )
    , consumptionAmount_  ( 0 )
    , consumptionCritical_( false )
    , modifier_           ( 1. ) 
{
    xis >> xml::list( "link", *this, &NodeElement::ReadLink );
}

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement( const NodeElement& from )
    : model_              ( 0 )
    , resourceType_       ( from.resourceType_ )
    , isActivated_        ( from.isActivated_ )
    , productionCapacity_ ( from.productionCapacity_ )
    , stockCapacity_      ( from.stockCapacity_ )
    , stockMaxCapacity_   ( from.stockMaxCapacity_ )
    , immediateStock_     ( from.immediateStock_ )
    , receivedQuantity_   ( from.receivedQuantity_ )
    , consumptionAmount_  ( from.consumptionAmount_ )
    , consumptionCritical_( from.consumptionCritical_ )
    , modifier_           ( from.modifier_ ) 
{
    for( CIT_ResourceLinks it = from.links_.begin(); it != from.links_.end(); ++it )
        links_.push_back( new ResourceLink( **it ) );
}

// -----------------------------------------------------------------------------
// Name: NodeElement destructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::~NodeElement()
{
    for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::SetModel
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::SetModel( const ResourceNetworkModel& model )
{
    if( model_ )
        throw std::exception( "Node element Model already defined" );
    model_ = &model;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Update
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::Update( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "activation" )
        >> xml::attribute( "enabled", isActivated_ )
        >> xml::end();
    xis >> xml::optional >> xml::start( "production" )
        >> xml::attribute( "capacity", productionCapacity_ )
        >> xml::end();
    xis >> xml::optional >> xml::start( "stock" )
        >> xml::attribute( "capacity", stockCapacity_ )
        >> xml::attribute( "max-capacity", stockMaxCapacity_ )
        >> xml::end();
    xis >> xml::optional >> xml::start( "links" )
            >> xml::list( "resource", *this, &NodeElement::ReadLink )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: NodeElement::ReadConsumption
// Created: JSR 2010-08-30
// -----------------------------------------------------------------------------
void NodeElement::ReadConsumption( xml::xistream& xis )
{
    consumptionAmount_ = xis.attribute< int >( "amount" );
    consumptionCritical_ = xis.attribute< bool >( "critical" );
}

// -----------------------------------------------------------------------------
// Name: NodeElement::UpdateImmediateStock
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::UpdateImmediateStock( bool isFunctional )
{
    if( !isActivated_ )
        return;
    immediateStock_ = static_cast< unsigned int >( modifier_ * receivedQuantity_ ) + stockCapacity_;
    if( isFunctional )
        immediateStock_ += static_cast< unsigned int >( modifier_ * productionCapacity_ );
    receivedQuantity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Consume
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::Consume( bool& isFunctional )
{
    if( !isActivated_ )
        return;
    unsigned int consumption = static_cast< unsigned int >( modifier_ * consumptionAmount_ );
    if( consumption > immediateStock_ )
    {
        immediateStock_ = 0;
        if( consumptionCritical_ )
            isFunctional = false;
    }
    immediateStock_ -= consumption;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::DistributeResource
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::DistributeResource( bool isFunctional )
{
    for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        ( *it )->SetFlow( 0 );
    if( !isActivated_ || !isFunctional )
        return;
    if( immediateStock_ != 0 && links_.size() > 0 )
    {
        T_ResourceLinks links = links_;
        DoDistributeResource( links );
    }
    // finally update stock
    stockCapacity_ = std::min( immediateStock_, static_cast< unsigned int >( modifier_ * stockMaxCapacity_ ) );
}

// -----------------------------------------------------------------------------
// Name: NodeElement::DoDistributeResource
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::DoDistributeResource( T_ResourceLinks& links )
{
    int distributionMean = static_cast< int >( static_cast< float >( immediateStock_ ) / links.size() );
    T_ResourceLinks updatedLinks;
    for( CIT_ResourceLinks it = links.begin(); it != links.end(); ++it )
    {
        // distribute to links with capacity inferior to mean
        int linkCapacity = ( *it )->GetEfficientCapacity();
        if( linkCapacity != -1 && linkCapacity <= distributionMean )
        {
            ( *it )->SetFlow( linkCapacity );
            model_->Push( ( *it )->GetDestination(), ( *it )->GetDestinationKind()== ResourceLink::eDestinationKindUrban, linkCapacity, resourceType_ );
            immediateStock_ -= linkCapacity;
            updatedLinks.push_back( *it );
        }
    }
    if( updatedLinks.empty() )
    {
        // every link capacity is infinite or superior to mean, distribute everything
        int residual = immediateStock_ - links.size() * distributionMean;
        immediateStock_ = 0;
        for( CIT_ResourceLinks it = links.begin(); it != links.end(); ++it )
        {
            int linkCapacity = ( *it )->GetEfficientCapacity();
            int distributed = distributionMean;
            if( residual > 0 )
            {
                if( linkCapacity == -1 )
                {
                    distributed += residual;
                    residual = 0;
                }
                else
                {
                    int excedent = std::min( residual, linkCapacity - distributionMean );
                    distributed += excedent;
                    residual -= excedent;
                }
            }
            ( *it )->SetFlow( distributed );
            model_->Push( ( *it )->GetDestination(), ( *it )->GetDestinationKind()== ResourceLink::eDestinationKindUrban, distributed, resourceType_ );
        }
    }
    else
    {
        // remove already updated links from list and apply again
        {
            T_ResourceLinks newLinks;
            std::set_difference( links.begin(), links.end(), updatedLinks.begin(), updatedLinks.end(),
                std::insert_iterator< T_ResourceLinks >( newLinks, newLinks.end() ) );
            links = newLinks;
        }
        if( links.size() > 0 )
            DoDistributeResource( links );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::Push( int quantity )
{
    receivedQuantity_ += isActivated_ ? quantity : 0;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::SetModifier
// Created: JSR 2010-08-31
// -----------------------------------------------------------------------------
void NodeElement::SetModifier( unsigned int modifier )
{
    modifier_ = 0.01 * modifier;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void NodeElement::Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& msg ) const
{
    // TODO switch à virer quand on utilisera à dictionnaire
    switch( resourceType_ )
    {
    default:
    case eResourceType_Water:
        msg.set_type( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::water );
        break;
    case eResourceType_Gaz:
        msg.set_type( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::gaz );
        break;
    case eResourceType_Electricity:
        msg.set_type( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork::electricity );
        break;
    }
    msg.set_enabled( isActivated_ );
    if( stockMaxCapacity_ > 0 )
    {
        msg.set_max_stock( stockMaxCapacity_ );
        msg.set_stock( stockCapacity_ );
    }
    if( productionCapacity_ > 0 )
        msg.set_production( productionCapacity_ );
    msg.set_consumption( consumptionAmount_ );
    msg.set_critical( consumptionCritical_ );
    for( CIT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        ( *it )->Serialize( *msg.add_link() );
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void NodeElement::Update( const Common::MsgMissionParameter_Value& msg )
{
    consumptionAmount_ = msg.list( 1 ).quantity();
    consumptionCritical_ = msg.list( 2 ).abool();
    isActivated_ = msg.list( 3 ).abool();
    productionCapacity_ = msg.list( 4 ).quantity();
    stockMaxCapacity_ = msg.list( 5 ).quantity();
    for( int i = 0; i < msg.list( 6 ).list_size(); ++ i )
    {
        const Common::MsgMissionParameter_Value& link = msg.list( 6 ).list( i );
        unsigned int index = link.list( 0 ).identifier();
        if( index < links_.size() )
            links_[ index ]->SetCapacity( link.list( 1 ).quantity() );
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::ReadLink
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void NodeElement::ReadLink( xml::xistream& xis )
{
    unsigned int destination = xis.attribute< unsigned int >( "destination" );
    int capacity = xis.attribute< int >( "capacity" );
    ResourceLink::EDestinationKind kind = ResourceLink::eDestinationKindUrban;
    if( xis.has_attribute( "kind" ) )
        kind = ResourceLink::FindDestinationKind( xis.attribute< std::string >( "kind" ) );
    for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
    {
        if( ( *it )->GetDestination() == destination && ( *it )->GetDestinationKind() == kind )
        {
            ( *it )->SetCapacity( capacity );
            return;
        }
    }
    links_.push_back( new ResourceLink( destination, kind, capacity ) );
}
