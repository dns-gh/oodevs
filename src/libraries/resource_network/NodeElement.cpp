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
#include "ENT/ENT_Tr.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement()
    : model_             ( 0 )
    , resourceType_      ( static_cast< E_ResourceType>( -1 ) )
    , isActivated_       ( true )
    , isProducer_        ( false )
    , isStockActive_     ( false )
    , productionCapacity_( -1 ) // infinite
    , stockCapacity_     ( 0 )
    , stockMaxCapacity_  ( - 1 ) // infinite
    , immediateStock_    ( 0 )
    , receivedQuantity_  ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement( xml::xistream& xis, E_ResourceType resourceType )
    : model_             ( 0 )
    , resourceType_      ( resourceType )
    , isActivated_       ( true )
    , isProducer_        ( xis.attribute< bool >( "producer" ) )
    , isStockActive_     ( false )
    , productionCapacity_( -1 ) // infinite
    , stockCapacity_     ( 0 )
    , stockMaxCapacity_  ( -1 ) // infinite
    , immediateStock_    ( 0 )
    , receivedQuantity_  ( 0 )
{
    xis >> xml::list( "link", *this, &NodeElement::ReadLink );
}

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement( const NodeElement& from )
    : model_             ( 0 )
    , resourceType_      ( from.resourceType_ )
    , isActivated_       ( from.isActivated_ )
    , isProducer_        ( from.isProducer_ )
    , isStockActive_     ( from.isStockActive_ )
    , productionCapacity_( from.productionCapacity_ )
    , stockCapacity_     ( from.stockCapacity_ )
    , stockMaxCapacity_  ( from.stockMaxCapacity_ )
    , immediateStock_    ( from.immediateStock_ )
    , receivedQuantity_  ( from.receivedQuantity_ )
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
    xis >> xml::optional
        >> xml::start( "activation" )
            >> xml::attribute( "enabled", isActivated_ )
        >> xml::end();
    if( xis.has_child( "production" ) )
    {
        isProducer_ = true;
        xis >> xml::start( "production" )
                >> xml::attribute( "capacity", productionCapacity_ )
            >> xml::end();
    }
    if( xis.has_child( "stock" ) )
    {
        isStockActive_ = true;
        xis >> xml::start( "stock" )
                >> xml::attribute( "capacity", stockCapacity_ )
                >> xml::attribute( "max-capacity", stockMaxCapacity_ )
            >> xml::end();
    }
    xis >> xml::optional 
        >> xml::start( "consumption" )
            >> xml::list( "resource", *this, &NodeElement::ReadConsumption )
        >> xml::end()
        >> xml::optional
        >> xml::start( "links" )
            >> xml::list( "resource", *this, &NodeElement::ReadLink )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: NodeElement::UpdateImmediateStock
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::UpdateImmediateStock()
{
    if( !isActivated_ )
        return;
    immediateStock_ = receivedQuantity_ + isStockActive_ ? stockCapacity_ : 0;
    if( isProducer_ )
    {
        if( productionCapacity_ == - 1 )
            immediateStock_ = -1;
        else
            immediateStock_ += productionCapacity_;
    }
    receivedQuantity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::AddConsumptions
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::AddConsumptions( T_Consumptions& consumptions )
{
    for( CIT_Consumptions it = consumptions_.begin(); it != consumptions_.end(); ++it )
    {
        if( consumptions.find( it->first ) != consumptions.end() )
            consumptions[ it->first ].critical_ |= it->second.critical_;
        else
            consumptions[ it->first ].critical_ = it->second.critical_;
        consumptions[ it->first ].amount_ += it->second.amount_;
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Consume
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
bool NodeElement::Consume( int consumption )
{
    if( !isActivated_ )
        return true;
    if( immediateStock_ == - 1 ) // infinite
        return true;
    immediateStock_ -= consumption;
    if( immediateStock_ < 0 )
    {
        immediateStock_ = 0;
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::DistributeResource
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::DistributeResource()
{
    if( !isActivated_ )
        return;
    if( immediateStock_ != 0 )
    {
        T_ResourceLinks links = links_;
        DoDistributeResource( links );
    }
    // finally update stock
    if( isStockActive_ && immediateStock_ >= 0 )
    {
        stockCapacity_ = immediateStock_;
        if( stockMaxCapacity_ != -1 && static_cast< int >( stockCapacity_ ) > stockMaxCapacity_ )
            stockCapacity_ = stockMaxCapacity_;
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::DoDistributeResource
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::DoDistributeResource( T_ResourceLinks& links )
{
    int totalFlow = 0;
    for( CIT_ResourceLinks it = links.begin(); it != links.end(); ++it )
        totalFlow += ( *it )->GetEfficientCapacity();
    if( immediateStock_ == -1 || totalFlow <= immediateStock_ )
    {
        // remaining stock is sufficient
        for( CIT_ResourceLinks it = links.begin(); it != links.end(); ++it )
            model_->Push( ( *it )->GetDestination(), ( *it )->GetDestinationKind()== ResourceLink::eDestinationKindUrban, ( *it )->GetEfficientCapacity(), resourceType_ );
        if( immediateStock_ != -1 )
            immediateStock_ -= totalFlow;
    }
    else
    {
        int distributionMean = static_cast< int >( static_cast< float >( immediateStock_ ) / links.size() );
        T_ResourceLinks updatedLinks;
        for( CIT_ResourceLinks it = links.begin(); it != links.end(); ++it )
        {
            // distribute to links with capacity inferior to mean
            int linkCapacity = ( *it )->GetEfficientCapacity();
            if( linkCapacity <= distributionMean )
            {
                model_->Push( ( *it )->GetDestination(), ( *it )->GetDestinationKind()== ResourceLink::eDestinationKindUrban, linkCapacity, resourceType_ );
                immediateStock_ -= linkCapacity;
                updatedLinks.push_back( *it );
            }
        }
        if( updatedLinks.empty() )
        {
            // every link has a capacity superior to mean, distribute everything
            int residual = immediateStock_ - links.size() * distributionMean;
            immediateStock_ = 0;
            for( CIT_ResourceLinks it = links.begin(); it != links.end(); ++it )
            {
                int linkCapacity = ( *it )->GetEfficientCapacity();
                int distributed = distributionMean;
                if( residual > 0 )
                {
                    int excedent = std::min( residual, linkCapacity - distributionMean );
                    distributed += excedent;
                    residual -= excedent;
                }
                model_->Push( ( *it )->GetDestination(), ( *it )->GetDestinationKind()== ResourceLink::eDestinationKindUrban, distributed, resourceType_ );
            }
        }
        else
        {
            // remove already updated links from list and apply again
            {
                T_ResourceLinks newLinks;
                std::set_difference( links.begin(), links.end(), updatedLinks.begin(), updatedLinks.end(), newLinks.begin() );
                links = newLinks;
            }
            DoDistributeResource( links );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Push
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::Push( int quantity )
{
    receivedQuantity_ = isActivated_ ? quantity : 0;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void NodeElement::Serialize( MsgsSimToClient::MsgUrbanAttributes_Infrastructures_ResourceNetwork& msg ) const
{
    msg.set_producer( isProducer_ );
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
    if( isStockActive_ )
        msg.set_stock( stockCapacity_ );
    for( CIT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        ( *it )->Serialize( *msg.add_link() );
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void NodeElement::Update( const Common::MsgMissionParameter_Value& msg )
{
    for( int i = 0; i < msg.list_size(); ++ i )
    {
        const Common::MsgMissionParameter_Value& link = msg.list( i );
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

// -----------------------------------------------------------------------------
// Name: NodeElement::ReadConsumption
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::ReadConsumption( xml::xistream& xis )
{
    E_ResourceType type = ENT_Tr::ConvertToResourceType( xis.attribute< std::string >( "type" ) );
    Consumption consumption;
    consumption.amount_ = xis.attribute< int >( "amount" );
    consumption.critical_ = xis.attribute< bool >( "critical" );
    consumptions_[ type ] = consumption;
}
