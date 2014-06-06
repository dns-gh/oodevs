// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Population.h"
#include "Model.h"
#include "Side.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "PopulationOrder.h"
#include "EntityPublisher.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::Population( Model_ABC& model, const sword::CrowdCreation& msg, const std::string& decisionalModel )
    : dispatcher::Population_ABC( msg.crowd().id(), QString( msg.name().c_str() ) )
    , model_           ( model )
    , nType_           ( msg.type().id() )
    , strName_         ( msg.name() )
    , side_            ( model.Sides().Get( msg.party().id() ) )
    , male_            ( msg.repartition().male() )
    , female_          ( msg.repartition().female() )
    , children_        ( msg.repartition().children() )
    , nDominationState_( 0 )
    , decisionalInfos_ ( model )
    , armedIndividuals_( 0 )
    , decisionalModel_ ( decisionalModel )
    , brainDebug_      ( false )
{
    if( msg.has_color() )
        color_.reset( new sword::RgbColor( msg.color() ) );
    side_.Register( *this );
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Population::~Population()
{
    side_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdCreation& /*message*/ )
{
    decisionalInfos_.Clear();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdUpdate& msg )
{
    if( msg.has_domination() )
        nDominationState_ = msg.domination();
    if( msg.has_adhesions() )
        for( int i = 0; i < msg.adhesions().adhesion_size(); ++i )
        {
            const sword::PartyAdhesion& adhesion = msg.adhesions().adhesion( i );
            affinities_[ adhesion.party().id() ] = adhesion.value();
        }
    if( msg.has_critical_intelligence() )
        criticalIntelligence_ = msg.critical_intelligence();
    if( msg.has_armed_individuals() )
        armedIndividuals_ = msg.armed_individuals();
    if( msg.has_extension() )
        for( int i = 0; i < msg.extension().entries_size(); ++i )
            extensions_[ msg.extension().entries( i ).name() ] = msg.extension().entries( i ).value();
    if( msg.has_decisional_model() )
        decisionalModel_ = msg.decisional_model();
    if( msg.has_brain_debug() )
        brainDebug_ = msg.brain_debug();
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdConcentrationCreation& msg )
{
    kernel::PopulationConcentration_ABC* element = const_cast< kernel::PopulationConcentration_ABC* >( FindConcentration( msg.concentration().id() ) );
    if( !element  )
    {
        PopulationConcentration* concentration = new PopulationConcentration( *this, msg );
        static_cast< Model* >( &model_ )->AddExtensions( *concentration ); // $$$$ SBO 2010-06-10: use population part factory or something
        tools::Resolver< kernel::PopulationConcentration_ABC >::Register( concentration->GetId(), *concentration );
        element = concentration;
    }
    element->Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdConcentrationUpdate& msg )
{
    const_cast< kernel::PopulationConcentration_ABC& >( GetConcentration( msg.concentration().id() ) ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdConcentrationDestruction& msg )
{
    if( const kernel::PopulationConcentration_ABC* concentration = FindConcentration( msg.concentration().id() ) )
    {
        tools::Resolver< kernel::PopulationConcentration_ABC >::Remove( msg.concentration().id() );
        delete concentration;
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdFlowCreation& msg )
{
    kernel::PopulationFlow_ABC* element = const_cast < kernel::PopulationFlow_ABC* > ( FindFlow( msg.flow().id() ) );
    if( !element )
    {
        PopulationFlow* flow = new PopulationFlow( *this, msg );
        static_cast< Model& >( model_ ).AddExtensions( *flow ); // $$$$ SBO 2010-06-10: use population part factory or something
        tools::Resolver< kernel::PopulationFlow_ABC >::Register( flow->GetId(), *flow );
        element = flow;
    }
   element->Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdFlowUpdate& msg )
{
    const_cast< kernel::PopulationFlow_ABC& >( GetFlow( msg.flow().id()) ).Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdFlowDestruction& msg )
{
    if( const kernel::PopulationFlow_ABC* flow = FindFlow( msg.flow().id() ) )
    {
        tools::Resolver< kernel::PopulationFlow_ABC >::Remove( msg.flow().id() );
        delete flow;
    }
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::CrowdOrder& message )
{
    order_.reset();
    if( message.type().id() != 0 )
        order_.reset( new PopulationOrder( message ) );
}

// -----------------------------------------------------------------------------
// Name: Population::DoUpdate
// Created: ZEBRE 2007-06-21
// -----------------------------------------------------------------------------
void Population::DoUpdate( const sword::DecisionalState& msg )
{
    decisionalInfos_.Update( msg );
}

// -----------------------------------------------------------------------------
// Name: Population::SendCreation
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::CrowdCreation asn;
    asn().mutable_crowd()->set_id( GetId() );
    asn().mutable_party()->set_id( side_.GetId() );
    asn().mutable_type()->set_id( nType_ );
    asn().set_name( strName_ );
    if( color_ )
        *asn().mutable_color() = *color_;
    asn().mutable_repartition()->set_male( male_ );
    asn().mutable_repartition()->set_female( female_ );
    asn().mutable_repartition()->set_children( children_ );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::SendFullUpdate
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void Population::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::CrowdUpdate asn;
    asn().mutable_crowd()->set_id( GetId() );
    asn().set_domination( nDominationState_ );
    for( auto it = affinities_.cbegin(); it != affinities_.cend(); ++it )
    {
        sword::PartyAdhesion& adhesion = *asn().mutable_adhesions()->add_adhesion();
        adhesion.mutable_party()->set_id( it->first );
        adhesion.set_value( it->second );
    }
    asn().set_critical_intelligence( criticalIntelligence_ );
    asn().set_armed_individuals( armedIndividuals_ );
    asn().set_healthy( GetHealthyHumans() );
    asn().set_wounded( GetWoundedHumans() );
    asn().set_contaminated( GetContaminatedHumans() );
    asn().set_dead( GetDeadHumans() );
    asn().set_decisional_model( decisionalModel_ );
    asn().set_brain_debug( brainDebug_ );

    for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it !=  extensions_.end(); ++it )
    {
        sword::Extension_Entry* entry = asn().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
    asn.Send( publisher );
    if( order_.get() )
        order_->Send( publisher );
    else
        PopulationOrder::SendNoMission( *this, publisher );
    decisionalInfos_.Send( GetId(), publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Population::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::CrowdDestruction asn;
    asn().mutable_crowd()->set_id( GetId() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Population::Accept
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void Population::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    {
        tools::Iterator< const kernel::PopulationConcentration_ABC& > it = tools::Resolver< kernel::PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            static_cast< const dispatcher::PopulationConcentration_ABC& >( it.NextElement() ).Accept( visitor );
    }
    {
        tools::Iterator< const kernel::PopulationFlow_ABC& > it = tools::Resolver< kernel::PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            static_cast< const dispatcher::PopulationFlow_ABC& >( it.NextElement() ).Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: Population::GetHealthyHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetHealthyHumans() const
{
    unsigned int nbr = 0;
    {
        tools::Iterator< const kernel::PopulationConcentration_ABC& > it = tools::Resolver< kernel::PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetHealthyHumans();
    }
    {
        tools::Iterator< const kernel::PopulationFlow_ABC& > it = tools::Resolver< kernel::PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetHealthyHumans();
    }
    return nbr;
}

// -----------------------------------------------------------------------------
// Name: Population::GetWoundedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetWoundedHumans() const
{
    unsigned int nbr = 0;
    {
        tools::Iterator< const kernel::PopulationConcentration_ABC& > it = tools::Resolver< kernel::PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetWoundedHumans();
    }
    {
        tools::Iterator< const kernel::PopulationFlow_ABC& > it = tools::Resolver< kernel::PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetWoundedHumans();
    }
    return nbr;
}

// -----------------------------------------------------------------------------
// Name: Population::GetContaminatedHumans
// Created: JSR 2011-03-11
// -----------------------------------------------------------------------------
unsigned int Population::GetContaminatedHumans() const
{
    unsigned int nbr = 0;
    {
        tools::Iterator< const kernel::PopulationConcentration_ABC& > it = tools::Resolver< kernel::PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetContaminatedHumans();
    }
    {
        tools::Iterator< const kernel::PopulationFlow_ABC& > it = tools::Resolver< kernel::PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetContaminatedHumans();
    }
    return nbr;
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
unsigned int Population::GetDeadHumans() const
{
    unsigned int nbr = 0;
    {
        tools::Iterator< const kernel::PopulationConcentration_ABC& > it = tools::Resolver< kernel::PopulationConcentration_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetDeadHumans();
    }
    {
        tools::Iterator< const kernel::PopulationFlow_ABC& > it = tools::Resolver< kernel::PopulationFlow_ABC >::CreateIterator();
        while( it.HasMoreElements() )
            nbr += it.NextElement().GetDeadHumans();
    }
    return nbr;
}

// -----------------------------------------------------------------------------
// Name: Population::GetOrder
// Created: NPT 2012-09-13
// -----------------------------------------------------------------------------
const Order_ABC* Population::GetOrder() const
{
    return order_.get();
}
