// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "NodeElement.h"
#include "ResourceNetworkModel.h"
#include "ResourceTools_ABC.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace resource;

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement()
    : model_              ( 0 )
    , resourceId_         ( 0 )
    , resourceName_       ()
    , isActivated_        ( true )
    , productionCapacity_ ( 0 )
    , stockCapacity_      ( 0 )
    , stockMaxCapacity_   ( 0 )
    , immediateStock_     ( 0 )
    , receivedQuantity_   ( 0 )
    , consumptionAmount_  ( 0 )
    , externalConsumption_( 0 )
    , consumptionCritical_( false )
    , modifier_           ( 1. )
    , needUpdate_         ( true )
    , functionalState_    ( 0 )
    , consumptionState_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement( xml::xistream& xis, unsigned long resourceId, const std::string& resourceName )
    : model_              ( 0 )
    , resourceId_         ( resourceId )
    , resourceName_       ( resourceName )
    , isActivated_        ( true )
    , productionCapacity_ ( 0 )
    , stockCapacity_      ( 0 )
    , stockMaxCapacity_   ( 0 )
    , immediateStock_     ( 0 )
    , receivedQuantity_   ( 0 )
    , consumptionAmount_  ( 0 )
    , externalConsumption_( 0 )
    , consumptionCritical_( false )
    , modifier_           ( 1. )
    , needUpdate_         ( true )
    , functionalState_    ( 0 )
    , consumptionState_   ( 0 )
{
    Update( xis );
}

// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-09-17
// -----------------------------------------------------------------------------
NodeElement::NodeElement( const urban::ResourceNetworkAttribute::ResourceNode& node, unsigned long resourceId )
    : model_              ( 0 )
    , resourceId_         ( resourceId )
    , resourceName_       ( node.resource_ )
    , isActivated_        ( node.isEnabled_ )
    , productionCapacity_ ( node.production_ )
    , stockCapacity_      ( 0 )
    , stockMaxCapacity_   ( node.maxStock_ )
    , immediateStock_     ( 0 )
    , receivedQuantity_   ( 0 )
    , consumptionAmount_  ( node.consumption_ )
    , externalConsumption_( 0 )
    , consumptionCritical_( node.critical_ )
    , modifier_           ( 1. )
    , needUpdate_         ( true )
    , functionalState_    ( 0 )
    , consumptionState_   ( 0 )
{
    for( std::vector< urban::ResourceNetworkAttribute::ResourceLink >::const_iterator it = node.links_.begin(); it != node.links_.end(); ++it )
        links_.push_back( new ResourceLink( it->id_, ResourceLink::eTargetKindUrban, it->capacity_ ) );
}


// -----------------------------------------------------------------------------
// Name: NodeElement constructor
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
NodeElement::NodeElement( const NodeElement& from )
    : model_              ( 0 )
    , resourceId_         ( from.resourceId_ )
    , resourceName_       ( from.resourceName_ )
    , isActivated_        ( from.isActivated_ )
    , productionCapacity_ ( from.productionCapacity_ )
    , stockCapacity_      ( from.stockCapacity_ )
    , stockMaxCapacity_   ( from.stockMaxCapacity_ )
    , immediateStock_     ( from.immediateStock_ )
    , receivedQuantity_   ( from.receivedQuantity_ )
    , consumptionAmount_  ( from.consumptionAmount_ )
    , externalConsumption_( from.externalConsumption_ )
    , consumptionCritical_( from.consumptionCritical_ )
    , modifier_           ( from.modifier_ )
    , needUpdate_         ( true )
    , functionalState_    ( 0 )
    , consumptionState_   ( 0 )
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
    xis >> xml::optional >> xml::attribute( "enabled", isActivated_ )
        >> xml::optional >> xml::attribute( "production", productionCapacity_ )
        >> xml::optional >> xml::attribute( "stock", stockMaxCapacity_ )
        >> xml::optional >> xml::attribute( "initial-stock", stockCapacity_ )
        >> xml::optional >> xml::attribute( "consumption", consumptionAmount_ )
        >> xml::optional >> xml::attribute( "critical-consumption", consumptionCritical_ )
        >> xml::list( "link", *this, &NodeElement::ReadLink );
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Finalize
// Created: JSR 2011-01-17
// -----------------------------------------------------------------------------
void NodeElement::Finalize( const ResourceTools_ABC& tools )
{
    for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        ( *it )->Finalize( tools );
}

// -----------------------------------------------------------------------------
// Name: NodeElement::UpdateImmediateStock
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::UpdateImmediateStock( float functionalState )
{
    if( !isActivated_ )
        return;
    immediateStock_ = static_cast< unsigned int >( modifier_ * receivedQuantity_ ) + stockCapacity_;
    if( std::abs( functionalState ) >= 0.01f )
        immediateStock_ += static_cast< unsigned int >( modifier_ * functionalState * productionCapacity_ );
    receivedQuantity_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::AddConsumption
// Created: JSR 2011-02-01
// -----------------------------------------------------------------------------
void NodeElement::AddConsumption( double consumption )
{
    externalConsumption_ += consumption;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Consume
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::Consume( float& functionalState )
{
    if( isActivated_ )
    {
        unsigned int consumption = static_cast< unsigned int >( modifier_ * consumptionAmount_ );
        if( consumption > 0 && consumption > immediateStock_ )
        {
            immediateStock_ = 0;
            functionalState_ = consumptionCritical_ ? static_cast< float >( immediateStock_ ) / consumption : 1;
            functionalState *= functionalState_;
        }
        else
        {
            functionalState_ = 1;
            immediateStock_ -= consumption;
        }
    }
    if( externalConsumption_ > 0 )
    {
        unsigned int externalConsumption = static_cast< unsigned int >( externalConsumption_ + 0.5 );
        if( externalConsumption == 0 || immediateStock_ > externalConsumption )
        {
            consumptionState_ = 1.f;
            immediateStock_ -= externalConsumption;
        }
        else
        {
            consumptionState_ = static_cast< float >( immediateStock_ ) / externalConsumption;
            immediateStock_ = 0;
        }
        externalConsumption_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: NodeElement::DistributeResource
// Created: JSR 2010-08-16
// -----------------------------------------------------------------------------
void NodeElement::DistributeResource( float functionalState )
{
    if( immediateStock_ == 0 || !isActivated_ || std::abs( functionalState ) < 0.01f )
    {
        for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
            ( *it )->SetFlow( 0 );
        return;
    }
    for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        ( *it )->ResetFlow();
    if( links_.size() > 0 )
    {
        T_ResourceLinks links = links_;
        DoDistributeResource( links );
    }
    // finally update stock
    int oldStockCapacity = stockCapacity_;
    stockCapacity_ = std::min( immediateStock_, static_cast< unsigned int >( modifier_ * stockMaxCapacity_ ) );
    if( ( stockMaxCapacity_ == 0 && oldStockCapacity != 0 ) || std::abs( static_cast< float >( oldStockCapacity - stockCapacity_ ) ) / stockMaxCapacity_ > 0.05f )
        needUpdate_ = true;
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
            model_->Push( ( *it )->GetTarget(), linkCapacity, resourceId_ );
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
            model_->Push( ( *it )->GetTarget(), distributed, resourceId_ );
        }
    }
    else
    {
        // remove already updated links from list and apply again
        {
            T_ResourceLinks newLinks;
            std::sort( links.begin(), links.end() );
            std::sort( updatedLinks.begin(), updatedLinks.end() );
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
void NodeElement::SetModifier( float modifier )
{
    modifier_ = modifier;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::NeedUpdate
// Created: JSR 2010-11-30
// -----------------------------------------------------------------------------
bool NodeElement::NeedUpdate() const
{
    if( needUpdate_ )
        return true;
    for( CIT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        if( ( *it )->NeedUpdate() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Serialize
// Created: JSR 2010-08-17
// -----------------------------------------------------------------------------
void NodeElement::Serialize( sword::ResourceNetwork& msg ) const
{
    msg.mutable_resource()->set_name( resourceName_ );
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
    needUpdate_ = false;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::Update
// Created: JSR 2010-08-26
// -----------------------------------------------------------------------------
void NodeElement::Update( const sword::MissionParameter_Value& msg )
{
    bool oldActivated = isActivated_;
    unsigned int oldProductionCapacity = productionCapacity_;
    unsigned int oldStockMaxCapacity = stockMaxCapacity_;
    unsigned int oldConsumptionAmount = consumptionAmount_;
    bool oldConsumptionCritical = consumptionCritical_;
    consumptionAmount_ = msg.list( 1 ).quantity();
    consumptionCritical_ = msg.list( 2 ).booleanvalue();
    isActivated_ = msg.list( 3 ).booleanvalue();
    productionCapacity_ = msg.list( 4 ).quantity();
    stockMaxCapacity_ = msg.list( 5 ).quantity();
    for( int i = 0; i < msg.list( 6 ).list_size(); ++ i )
    {
        const sword::MissionParameter_Value& link = msg.list( 6 ).list( i );
        unsigned int index = link.list( 0 ).identifier();
        if( index < links_.size() )
            links_[ index ]->SetCapacity( link.list( 1 ).quantity() );
    }
    if( oldActivated != isActivated_ || oldProductionCapacity != productionCapacity_ || oldStockMaxCapacity != stockMaxCapacity_
        || oldConsumptionAmount !=  consumptionAmount_ || oldConsumptionCritical != consumptionCritical_ )
        needUpdate_ = true;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::ReadLink
// Created: JSR 2010-08-13
// -----------------------------------------------------------------------------
void NodeElement::ReadLink( xml::xistream& xis )
{
    unsigned int target = xis.attribute< unsigned int >( "target" );
    int capacity = xis.attribute< int >( "capacity" );
    ResourceLink::ETargetKind kind = ResourceLink::eTargetKindUrban;
    if( xis.has_attribute( "kind" ) )
        kind = ResourceLink::FindTargetKind( xis.attribute< std::string >( "kind" ) );
    for( IT_ResourceLinks it = links_.begin(); it != links_.end(); ++it )
        if( ( *it )->GetTarget() == target && ( *it )->GetTargetKind() == kind )
        {
            ( *it )->SetCapacity( capacity );
            return;
        }
    links_.push_back( new ResourceLink( target, kind, capacity ) );
}


// -----------------------------------------------------------------------------
// Name: NodeElement::GetFunctionalState
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
float NodeElement::GetFunctionalState() const
{
    return functionalState_;
}

// -----------------------------------------------------------------------------
// Name: NodeElement::GetConsumptionState
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
float NodeElement::GetConsumptionState() const
{
    return consumptionState_;
}
