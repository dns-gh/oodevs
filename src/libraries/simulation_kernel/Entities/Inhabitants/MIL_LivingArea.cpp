// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LivingArea.h"
#include "MIL_LivingAreaBlock.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Inhabitants/MIL_Inhabitant.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/InfrastructureCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "protocol/ClientSenders.h"
#include "Urban/PHY_AccomodationType.h"
#include "simulation_terrain/TER_Geometry.h"
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LivingArea )

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea constructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::MIL_LivingArea()
    : pInhabitant_( 0 )
    , population_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea constructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::MIL_LivingArea( xml::xistream& xis, unsigned long population, MIL_Inhabitant& inhabitant )
    : pInhabitant_( &inhabitant )
    , population_ ( population )
{
    xis >> xml::start( "living-area" )
            >> xml::list( "urban-block", *this, &MIL_LivingArea::ReadUrbanBlock )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void MIL_LivingArea::Finalize()
{
    for( auto it = urbanIds_.begin(); it != urbanIds_.end(); ++it )
    {
        const unsigned int simId = MIL_AgentServer::GetWorkspace().GetEntityManager().ConvertUrbanIdToSimId( *it );
        if( simId != 0)
        {
            MIL_UrbanObject_ABC* object = dynamic_cast< MIL_UrbanObject_ABC* >( MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( simId ) );
            if( !object )
                throw MASA_EXCEPTION( "Error in loading living urban block of population" );
            blocks_.push_back( new MIL_LivingAreaBlock( *object ) );
            object->MIL_Object::Register( *pInhabitant_ );
            object->AddLivingArea( *this );
        }
    }
    DistributeHumans( population_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea destructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::~MIL_LivingArea()
{
    for( auto it = blocks_.begin(); it != blocks_.end(); ++it )
        delete *it;
}

namespace
{
    float GetStructuralState( const MIL_UrbanObject_ABC& object )
    {
        const StructuralCapacity* structural = object.Retrieve< StructuralCapacity >();
        return structural ? structural->GetStructuralState() : 1.f;
    }

    template< typename T >
    bool CompareLivingSpace( const T* lhs, const T* rhs )
    {
        return lhs->GetObject().GetLivingSpace() * GetStructuralState( lhs->GetObject() ) > rhs->GetObject().GetLivingSpace() * GetStructuralState( rhs->GetObject() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::serialize
// Created: JSR 2011-03-29
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_LivingArea::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< MIL_LivingArea_ABC >( *this );
    file & population_
         & pInhabitant_
         & blocks_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ReadUrbanBlock
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::ReadUrbanBlock( xml::xistream& xis )
{
    urbanIds_.insert( xis.attribute< unsigned int >( "id" ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::DistributeHumans
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::DistributeHumans( unsigned long population )
{
    float area = 0;
    for( auto it = blocks_.begin(); it != blocks_.end(); ++it )
        area += ( *it )->GetObject().GetLivingSpace() * GetStructuralState( ( *it )->GetObject() );
    population_ = population;
    std::sort( blocks_.begin(), blocks_.end(), boost::bind( &CompareLivingSpace< MIL_LivingAreaBlock >, _1, _2 ) );
    unsigned long tmp = population_;
    for( auto it = blocks_.begin(); it != blocks_.end() && tmp > 0; ++it )
    {
        float ratio = ( *it )->GetObject().GetLivingSpace() * GetStructuralState( ( *it )->GetObject() ) / area;
        unsigned long person = static_cast< unsigned long >( ratio * population_ );
        if( tmp - person < 0 )
            person = tmp;
        if( ( it + 1 ) == blocks_.end() && tmp > 0 )
            ( *it )->DistributeHumans( tmp, *this );
        else
            ( *it )->DistributeHumans( person, *this );
        tmp -= person;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SendFullState
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::SendFullState( client::PopulationUpdate& msg ) const
{
    BOOST_FOREACH( const MIL_LivingAreaBlock* urbanBlock, blocks_ )
        urbanBlock->SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::UpdateNetwork
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_LivingArea::UpdateNetwork( client::PopulationUpdate& msg ) const
{
    BOOST_FOREACH( const MIL_LivingAreaBlock* urbanBlock, blocks_ )
        urbanBlock->UpdateNetwork( msg );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SendCreation
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::SendCreation( client::PopulationCreation& msg ) const
{
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
        msg().add_objects()->set_id( block->GetObject().GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::WriteODB
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "living-area" );
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
    {
        xos << xml::start( "urban-block" )
                << xml::attribute( "id", block->GetObject().GetUrbanId() )
            << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ComputeOccupationFactor
// Created: JSR 2011-02-02
// -----------------------------------------------------------------------------
float MIL_LivingArea::ComputeOccupationFactor() const
{
    if( population_ == 0 )
        return 1.f;
    float occupationFactor = 0;
    for( auto it = blocks_.begin(); it != blocks_.end(); ++it )
        occupationFactor += ( *it )->ComputeOccupationFactor();
    return occupationFactor / population_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetUsagesOccupation
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_LivingArea::GetUsagesOccupation( std::map< std::string, unsigned int >& occupations ) const
{
    // $$$$ JSR 2011-03-22: à vérifier pour la satisfaction
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
        PHY_AccomodationType::Visit( [&]( const PHY_AccomodationType& type ){
            occupations[ type.GetRole() ] += block->GetNominalOccupation( type.GetRole(), &type );
        });
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Consume
// Created: JSR 2011-02-01
// -----------------------------------------------------------------------------
float MIL_LivingArea::Consume( const PHY_ResourceNetworkType& resource, unsigned int consumption, T_Blocks& angryBlocks )
{
    if( population_ == 0 )
        return 1.f;
    float satisfaction = 0;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
    {
        bool angry = false;
        satisfaction += block->Consume( resource, consumption, angry );
        if( angry )
            angryBlocks.push_back( block );
    }
    return satisfaction / population_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::HealthCount
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
float MIL_LivingArea::HealthCount() const
{
    float healthCount = 0;
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
        if( block->GetObject().Retrieve< MedicalCapacity >() )
        {
            const InfrastructureCapacity* infrastructure = block->GetObject().Retrieve< InfrastructureCapacity >();
            if( !infrastructure || infrastructure->IsActive() )
                healthCount += GetStructuralState( block->GetObject() );
        }
    return healthCount;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::StartMotivation
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::StartMotivation( const std::string& motivation )
{
    Clean();
    // calcul du nombre de gens à déplacer et de la liste de départ
    unsigned int movingNumber = ComputeStartingBlocks( motivation );
    if( movingNumber == 0 )
        return;

    // destinations
    unsigned int nominalRemaining = 0;
    unsigned int maximalRemaining = 0;
    std::map< MIL_LivingAreaBlock*, std::pair< unsigned int, unsigned int > > blocksFreeSpaces;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
    {
        if( !block->CanMove() || block->IsEvacuated() )
            continue;
        unsigned int people = block->GetObject().GetTotalInhabitantsForMotivation( motivation );
        unsigned int nominalOccupation = block->GetNominalOccupation( motivation );
        unsigned int maximalOccupation = block->GetMaxOccupation( motivation );
        if( nominalOccupation != 0 || maximalOccupation != 0 )
        {
            unsigned int blockNominalRemaining = nominalOccupation > people ? nominalOccupation - people : 0;
            unsigned int blockMaximalRemaining = maximalOccupation > people ? maximalOccupation - people : 0;
            blocksFreeSpaces[ block ] = std::make_pair< unsigned int, unsigned int >( blockNominalRemaining, blockMaximalRemaining ) ;
            nominalRemaining += blockNominalRemaining;
            maximalRemaining += blockMaximalRemaining;
        }
    }
    if( movingNumber <= nominalRemaining )
    {
        for( auto it = blocksFreeSpaces.begin(); it != blocksFreeSpaces.end(); ++it )
            finalBlocks_[ it->first ].first = static_cast< float >( it->second.first ) / nominalRemaining;
    }
    else if( movingNumber <= maximalRemaining )
    {
        for( auto it = blocksFreeSpaces.begin(); it != blocksFreeSpaces.end(); ++it )
            finalBlocks_[ it->first ].first = static_cast< float >( it->second.second ) / maximalRemaining;
    }
    else
    {
        if( maximalRemaining == 0 )
        {
            for( auto it = startingBlocks_.begin(); it != startingBlocks_.end(); ++it )
                for( auto itMotivation = it->second.begin(); itMotivation != it->second.end(); ++itMotivation )
                    itMotivation->second = 0;
        }
        else
        {
            float ratio = static_cast< float >( maximalRemaining ) / movingNumber;
            movingNumber = maximalRemaining;
            for( auto it = startingBlocks_.begin(); it != startingBlocks_.end(); ++it )
                for( auto itMotivation = it->second.begin(); itMotivation != it->second.end(); ++itMotivation )
                {
                    unsigned int person = static_cast< unsigned int >( ratio * itMotivation->second + 1.f );
                    person = std::min( it->first->GetMaxOccupation( itMotivation->first ), std::min( movingNumber, person ) );
                    itMotivation->second = person;
                    movingNumber -= person;
                }
            assert( movingNumber == 0 );
            for( auto it = blocksFreeSpaces.begin(); it != blocksFreeSpaces.end(); ++it )
                finalBlocks_[ it->first ].first = static_cast< float >( it->second.second ) / maximalRemaining;
        }

        ForceEvacuation( motivation );

    }
    if( !finalBlocks_.empty() )
        currentStartingState_ = startingBlocks_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::MovePeople
// Created: SLG 2011-02-03
// -----------------------------------------------------------------------------
void MIL_LivingArea::MovePeople( const std::string& motivation, int occurence )
{
    if( finalBlocks_.empty() )
        return;
    unsigned int totalLeaving = 0;
    for( auto it = startingBlocks_.begin(); it != startingBlocks_.end(); ++it )
        for( auto accomodation = it->second.begin(); accomodation != it->second.end(); ++accomodation )
        {
            unsigned int leaving = static_cast< unsigned int >( static_cast< float >( accomodation->second ) / occurence );
            totalLeaving += leaving;
            it->first->DecreasePeopleWhenMoving( accomodation->first, leaving, *this );
            currentStartingState_[ it->first ][ accomodation->first ] -= leaving;
        }
    unsigned int tmp = totalLeaving;
    for( auto it = finalBlocks_.begin(); it != finalBlocks_.end() && tmp > 0; ++it )
    {
        unsigned int arriving = std::min( tmp, static_cast< unsigned int >( it->second.first * totalLeaving + 0.5f ) );
        tmp -= arriving;
        it->first->IncreasePeopleWhenMoving( motivation, arriving, *this );
        arriving = std::min( tmp, static_cast< unsigned int >( it->second.second * totalLeaving + 0.5f ) );
        tmp -= arriving;
        it->first->IncreasePeopleWhenMoving( "", arriving, *this );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::FinishMoving
// Created: SLG 2011-02-03
// -----------------------------------------------------------------------------
void MIL_LivingArea::FinishMoving( const std::string& motivation )
{
    if( finalBlocks_.empty() )
        return;
    unsigned int totalLeaving = 0;
    for( auto it = startingBlocks_.begin(); it != startingBlocks_.end(); ++it )
        for( auto accomodation = it->second.begin(); accomodation != it->second.end(); ++accomodation )
        {
            unsigned int leaving = currentStartingState_[ it->first ][ accomodation->first ];
            totalLeaving += leaving;
            it->first->DecreasePeopleWhenMoving( accomodation->first, leaving, *this );
        }
    unsigned int tmp = totalLeaving;
    unsigned int remaining = 0;
    for( auto it = finalBlocks_.begin(); it != finalBlocks_.end() && tmp > 0; ++it )
    {
        unsigned int arriving = std::min( tmp, static_cast< unsigned int >( it->second.first * totalLeaving + 0.5f ) );
        tmp -= arriving;
        remaining += it->first->IncreasePeopleWhenMoving( motivation, arriving, *this );
        arriving = std::min( tmp, static_cast< unsigned int >( it->second.second * totalLeaving + 0.5f ) );
        tmp -= arriving;
        remaining += it->first->IncreasePeopleWhenMoving( "", arriving, *this );
    }
    for( auto it = finalBlocks_.begin(); it != finalBlocks_.end() && remaining > 0; ++it )
    {
        remaining = it->first->IncreasePeopleWhenMoving( motivation, remaining, *this );
        remaining = it->first->IncreasePeopleWhenMoving( "", remaining, *this );
    }
    Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Clean
// Created: JSR 2011-03-25
// -----------------------------------------------------------------------------
void MIL_LivingArea::Clean()
{
    startingBlocks_.clear();
    currentStartingState_.clear();
    finalBlocks_.clear();
}
// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Alert
// Created: BCI 2011-02-01
// -----------------------------------------------------------------------------
void MIL_LivingArea::Alert( const TER_Localisation& localisation, bool status )
{
    bool hasBeenAlerted = false;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( block->GetObject().Intersect2DWithLocalisation( localisation ) )
        {
            block->SetAlerted( status );
            hasBeenAlerted = true;
        }
    assert( pInhabitant_ );
    if( hasBeenAlerted )
        pInhabitant_->ReStartMotivation();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::IsAlerted
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
bool MIL_LivingArea::IsAlerted( const TER_Localisation& localisation ) const
{
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
        if( block->IsAlerted( localisation ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SetAlerted
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
void MIL_LivingArea::SetAlerted( bool alerted, MIL_UrbanObject_ABC* pUrbanObject /*= 0*/ )
{
    bool hasBeenAlerted = false;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( pUrbanObject == 0 || pUrbanObject == &block->GetObject() )
        {
            block->SetAlerted( alerted );
            hasBeenAlerted = true;
        }
    assert( pInhabitant_ );
    if( hasBeenAlerted )
        pInhabitant_->ReStartMotivation();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SetConfined
// Created: BCI 2011-02-22
// -----------------------------------------------------------------------------
void MIL_LivingArea::SetConfined( bool confined, MIL_UrbanObject_ABC* pUrbanObject /*= 0*/ )
{
    bool hasBeenConfined = false;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( pUrbanObject == 0 || pUrbanObject == &block->GetObject() )
        {
            block->SetConfined( confined );
            hasBeenConfined = true;
        }
    assert( pInhabitant_ );
    if( hasBeenConfined )
        pInhabitant_->ReStartMotivation();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::IsConfined
// Created: CCD 2012-01-10
// -----------------------------------------------------------------------------
bool MIL_LivingArea::IsConfined( const TER_Localisation& localisation ) const
{
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
        if( block->IsConfined( localisation ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Confine
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
void MIL_LivingArea::Confine( const TER_Localisation& localisation, bool status )
{
    bool hasBeenConfined = false;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( block->GetObject().Intersect2DWithLocalisation( localisation ) )
        {
            block->SetConfined( status );
            hasBeenConfined = true;
        }
    assert( pInhabitant_ );
    if( hasBeenConfined )
        pInhabitant_->ReStartMotivation();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Evacuated
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
void MIL_LivingArea::Evacuate( const TER_Localisation& localisation, bool status )
{
    bool hasBeenConfined = false;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( block->GetObject().Intersect2DWithLocalisation( localisation ) )
        {
            block->SetEvacuated( status );
            hasBeenConfined = true;
        }
    assert( pInhabitant_ );
    if( hasBeenConfined )
        pInhabitant_->ReStartMotivation();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SetEvacuated
// Created: ABR 2011-03-23
// -----------------------------------------------------------------------------
void MIL_LivingArea::SetEvacuated( bool evacuated, MIL_UrbanObject_ABC* pUrbanObject /*= 0*/ )
{
    bool hasBeenEvacuated = false;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( pUrbanObject == 0 || pUrbanObject == &block->GetObject() )
        {
            block->SetEvacuated( evacuated );
            hasBeenEvacuated = true;
        }
    assert( pInhabitant_ );
    if( hasBeenEvacuated )
        pInhabitant_->ReStartMotivation();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::IsEvacuated
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
bool MIL_LivingArea::IsEvacuated( const TER_Localisation& localisation ) const
{
    BOOST_FOREACH( const MIL_LivingAreaBlock* block, blocks_ )
        if( block->IsEvacuated( localisation ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ComputeLivingArea
// Created: SLG 2011-01-26
// -----------------------------------------------------------------------------
T_PointVector MIL_LivingArea::ComputeMovingArea() const
{
    // $$$$ _RC_ JSR 2011-03-24: A vérifier, et à appeler du schedule
    T_PointVector hull;
    if( startingBlocks_.empty() || finalBlocks_.empty() )
        return hull;
    T_PointVector vertices;
    for( auto it = startingBlocks_.begin(); it != startingBlocks_.end(); ++it )
    {
        const T_PointVector& objectVertices = it->first->GetObject().GetLocalisation().GetPoints();
        vertices.insert( vertices.end(), objectVertices.begin(), objectVertices.end() );
    }
    for( auto it = finalBlocks_.begin(); it != finalBlocks_.end(); ++it )
    {
        const T_PointVector& objectVertices = it->first->GetObject().GetLocalisation().GetPoints();
        vertices.insert( vertices.end(), objectVertices.begin(), objectVertices.end() );
    }
    TER_Geometry::ComputeHull( hull, vertices );
    return hull;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetNonConfinedBlocks
// Created: BCI 2011-02-22
// -----------------------------------------------------------------------------
const MIL_LivingArea::T_Blocks MIL_LivingArea::GetNonConfinedBlocks() const
{
    T_Blocks results;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
        if( block->CanMove() )
            results.push_back( block );
    return results;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ComputeStartingBlocks
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
unsigned int MIL_LivingArea::ComputeStartingBlocks( const std::string& motivation )
{
    unsigned int movingNumber = 0;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
    {
        if( !block->CanMove() )
            continue;
        PHY_AccomodationType::Visit( [&]( const PHY_AccomodationType& type ){
            const auto& role = type.GetRole();
            if( motivation == role )
                return;
            const unsigned int nbrForAccommodation = block->GetPersonsForAccomodation( role );
            if( nbrForAccommodation == 0 )
                return;
            startingBlocks_[ block ][ role ] = nbrForAccommodation;
            movingNumber += nbrForAccommodation;
        });
    }
    return movingNumber;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ComputeEvacuatedPeople
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
unsigned int MIL_LivingArea::ComputeEvacuatedPeople()
{
    int evacuated = 0;
    for( auto it = startingBlocks_.begin(); it != startingBlocks_.end(); ++it )
    {
        if( !it->first->IsEvacuated() || it->first->GetTotalNumberOfPersons() == 0 )
            continue;
        for( auto itMotivation = it->second.begin(); itMotivation != it->second.end(); ++itMotivation )
        {
            int oldToMove = itMotivation->second;
            itMotivation->second = it->first->GetPersonsForAccomodation( itMotivation->first );
            evacuated += itMotivation->second - oldToMove;
        }
    }
    return evacuated;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ForceEvacuation
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
void MIL_LivingArea::ForceEvacuation( const std::string& motivation )
{
    if( ComputeEvacuatedPeople() == 0 )
        return;

    std::map< MIL_LivingAreaBlock*, unsigned int > freeSpaces;
    int forcedMaximalRemaining = 0;
    BOOST_FOREACH( MIL_LivingAreaBlock* block, blocks_ )
    {
        if( !block->CanMove() || block->IsEvacuated() )
            continue;
        unsigned int people = 0;
        unsigned int maximalOccupation = 0;
        PHY_AccomodationType::Visit( [&]( const PHY_AccomodationType& type ){
            const auto& role = type.GetRole();
            if( role == motivation )
                return;
            people += block->GetPersonsForAccomodation( role );
            maximalOccupation += block->GetMaxOccupation( role );
        });
        if( maximalOccupation != 0 )
        {
            unsigned int blockMaximalRemaining = maximalOccupation > people ? maximalOccupation - people : 0;
            freeSpaces[ block ] = blockMaximalRemaining;
            forcedMaximalRemaining += blockMaximalRemaining;
        }
    }

    for( auto it = freeSpaces.begin(); it != freeSpaces.end(); ++it )
        finalBlocks_[ it->first ].second = static_cast< float >( it->second ) / forcedMaximalRemaining;
}
