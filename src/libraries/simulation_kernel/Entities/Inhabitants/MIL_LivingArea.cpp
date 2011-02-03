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
#include "MIL_AgentServer.h"
#include "PHY_ResourceNetworkType.h"
#include "UrbanType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/ResourceNetworkCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <urban/StaticModel.h>
#include <urban/MotivationType.h>
#include <urban/MotivationsVisitor_ABC.h>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LivingArea )

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea constructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::MIL_LivingArea()
    : hasChanged_( false )
{
    LoadAccommodations();
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea constructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::MIL_LivingArea( xml::xistream& xis, unsigned long population )
    : population_( population )
    , hasChanged_( false )
    , area_      ( 0.f )
{
    xis >> xml::start( "living-area" )
            >> xml::list( "urban-block", *this, &MIL_LivingArea::ReadUrbanBlock )
        >> xml::end;
    LoadAccommodations();
    DistributeHumans( population );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea destructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::~MIL_LivingArea()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::LoadAccommodations
// Created: LGY 2011-01-26
// -----------------------------------------------------------------------------
void MIL_LivingArea::LoadAccommodations()
{
    tools::Iterator< const urban::MotivationType& > it = UrbanType::GetUrbanType().GetStaticModel().CreateIterator< urban::MotivationType >();
    while( it.HasMoreElements() )
    {
        const urban::MotivationType& type = it.NextElement();
        accommodations_[ type.GetName() ] = type.GetCapacity();
    }
}

namespace
{
    float GetStructuralState( const UrbanObjectWrapper& object )
    {
        const StructuralCapacity* structural = object.Retrieve< StructuralCapacity >();
        return structural ? 0.01f * structural->GetStructuralState() : 1.f;
    }

    template< typename T >
    bool CompareLivingSpace( const T& lhs, const T& rhs )
    {
        return lhs.first->GetLivingSpace() * GetStructuralState( *lhs.first ) > rhs.first->GetLivingSpace() * GetStructuralState( *rhs.first );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::ReadUrbanBlock
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::ReadUrbanBlock( xml::xistream& xis )
{
    const unsigned int simId = MIL_AgentServer::GetWorkspace().GetEntityManager().ConvertUrbanIdToSimId( xis.attribute< unsigned int >( "id" ) );
    UrbanObjectWrapper* object = dynamic_cast< UrbanObjectWrapper* >( MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( simId ) );
    if( !object )
        xis.error( "Error in loading living urban block of population" );
    area_ += object->GetLivingSpace() * GetStructuralState( *object );
    blocks_.push_back( T_Block( object , 0 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::DistributeHumans
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::DistributeHumans( unsigned long population )
{
    population_ = population;
    std::sort( blocks_.begin(), blocks_.end(), boost::bind( &CompareLivingSpace< T_Block >, _1, _2 ) );
    unsigned long tmp = population_;
    for( IT_Blocks it = blocks_.begin(); it != blocks_.end() && tmp > 0; ++it )
    {
        unsigned long person = static_cast< unsigned long >( it->first->GetLivingSpace() * GetStructuralState( *it->first ) * population_ / area_ );
        if( tmp - person < 0 )
            person = tmp;
        it->second = person;
        tmp -= person;
    }
    if( tmp > 0 && !blocks_.empty() )
        blocks_.front().second += tmp;
    hasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Register
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::Register( MIL_StructuralStateNotifier_ABC& structural )
{
    BOOST_FOREACH( T_Block& block, blocks_ )
        block.first->Register( structural );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::load
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_LivingArea_ABC >( *this );
    file >> population_;
    unsigned int size;
    file >> size;
    unsigned int objectId;
    unsigned int person;
    for( unsigned int i = 0; i < size; ++i )
    {
        file >> objectId
             >> person;
        UrbanObjectWrapper* object = dynamic_cast< UrbanObjectWrapper* >( MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( objectId ) );
        if( object )
            blocks_.push_back( T_Block( object , person ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::save
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_LivingArea_ABC >( *this );
    file << population_;
    unsigned int size = blocks_.size();
    file << size;
    unsigned int id;
    BOOST_FOREACH( const T_Block& block, blocks_ )
    {
        id = block.first->GetID();
        file << id
             << block.second;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SendFullState
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::SendFullState( client::PopulationUpdate& msg ) const
{
    BOOST_FOREACH( const T_Block& urbanBlock, blocks_ )
    {
        sword::PopulationUpdate_BlockOccupation& block = *msg().mutable_occupations()->Add();
        block.mutable_object()->set_id( urbanBlock.first->GetID() );
        block.set_number( urbanBlock.second );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::UpdateNetwork
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_LivingArea::UpdateNetwork( client::PopulationUpdate& msg ) const
{
    if( hasChanged_ )
    {
        SendFullState( msg );
        hasChanged_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SendCreation
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::SendCreation( client::PopulationCreation& msg ) const
{
    BOOST_FOREACH( const T_Block& block, blocks_ )
        msg().add_objects()->set_id( block.first->GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::WriteODB
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "living-area" );
    BOOST_FOREACH( const T_Block& block, blocks_ )
    {
        xos << xml::start( "urban-block" );
            block.first->WriteUrbanIdAttribute( xos );
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetTotalOccupation
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
unsigned int MIL_LivingArea::GetTotalOccupation() const
{
    unsigned int totalOccupation = 0;
    BOOST_FOREACH( const T_Block& block, blocks_ )
        for( CIT_Accommodations it = accommodations_.begin(); it != accommodations_.end(); ++it )
            if( block.second != 0 )
                totalOccupation += GetOccupation( block, it->first );
    return totalOccupation;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetUsagesOccupation
// Created: JSR 2011-01-27
// -----------------------------------------------------------------------------
void MIL_LivingArea::GetUsagesOccupation( std::map< std::string, unsigned int >& occupations ) const
{
    BOOST_FOREACH( const T_Block& block, blocks_ )
        for( CIT_Accommodations it = accommodations_.begin(); it != accommodations_.end(); ++it )
            occupations[ it->first ] += GetOccupation( block, it->first );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Consume
// Created: JSR 2011-02-01
// -----------------------------------------------------------------------------
float MIL_LivingArea::Consume( const PHY_ResourceNetworkType& resource, unsigned int consumption )
{
    if( population_ == 0 )
        return 1.f;
    double personalConsumption = static_cast< double >( consumption ) / population_;
    float satisfaction = 0;
    BOOST_FOREACH( const T_Block& block, blocks_ )
        if( block.second > 0 )
            if( ResourceNetworkCapacity* capacity = block.first->Retrieve< ResourceNetworkCapacity >() )
            {
                satisfaction += block.second * capacity->GetConsumptionState( resource.GetId() );
                capacity->AddConsumption( resource.GetId(), static_cast< unsigned int >( block.second * personalConsumption + 0.5 ) );
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
    BOOST_FOREACH( const T_Block& block, blocks_ )
        if( block.first->Retrieve< MedicalCapacity >() )
            healthCount += GetStructuralState( *block.first);
    return healthCount;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::StartMotivation
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::StartMotivation( const std::string& motivation )
{
    T_Blocks blocks = GetBlockUsage( motivation );
    if( !blocks.empty() )
    {
        unsigned int occupation = 0u;
        BOOST_FOREACH( const T_Block& block, blocks )
            occupation += GetOccupation( block, motivation );
        if( occupation != 0u )
        {
            T_Identifiers identifiers;
            unsigned long tmp = population_;
            BOOST_FOREACH( const T_Block& block, blocks )
            {
                unsigned int part = static_cast< unsigned int >( population_ * GetOccupation( block, motivation ) / occupation );
                identifiers[ block.first->GetID() ] = part;
                tmp -= part;
            }
            if( tmp > 0 )
                identifiers.begin()->second += tmp;
            BOOST_FOREACH( T_Block& block, blocks_ )
            {
                CIT_Identifiers it = identifiers.find( block.first->GetID() );
                block.second = ( it == identifiers.end() ) ? 0u : it->second;
            }
            hasChanged_ = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetOccupation
// Created: LGY 2011-01-26
// -----------------------------------------------------------------------------
unsigned int MIL_LivingArea::GetOccupation( const T_Block& block, const std::string& motivation ) const
{
    CIT_Accommodations it = accommodations_.find( motivation );
    if( it != accommodations_.end() )
        return static_cast< unsigned int >( block.first->GetLivingSpace() * GetStructuralState( *block.first ) * GetProportion( block, motivation ) * it->second );
    return 0u;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetBlockUsage
// Created: LGY 2011-01-21
// -----------------------------------------------------------------------------
MIL_LivingArea::T_Blocks MIL_LivingArea::GetBlockUsage( const std::string& motivation ) const
{
    T_Blocks blocks;
    BOOST_FOREACH( const T_Block& block, blocks_ )
        if( GetProportion( block, motivation ) != 0.f )
            blocks.push_back( block );
    return blocks;
}

namespace
{
    class MotivationsVisitor : public urban::MotivationsVisitor_ABC
    {
    public:
        explicit MotivationsVisitor( std::map< std::string, float >& motivations )
            : motivations_( motivations )
        {
            // NOTHING
        }
        virtual void Visit( const std::string& motivation, float proportion )
        {
            motivations_[ motivation ] = proportion;
        }
        std::map< std::string, float >& motivations_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetProportion
// Created: LGY 2011-01-26
// -----------------------------------------------------------------------------
float MIL_LivingArea::GetProportion( const T_Block& block, const std::string& motivation ) const
{
    T_Accommodations motivations;
    MotivationsVisitor visitor( motivations );
    block.first->Accept( visitor );
    CIT_Accommodations it = motivations.find( motivation );
    if( it == motivations.end() )
        return 0.f;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::NotifyAlerted
// Created: BCI 2011-02-01
// -----------------------------------------------------------------------------
bool MIL_LivingArea::Intersect2DWithLocalisation( const TER_Localisation& localisation ) const
{
    BOOST_FOREACH( const T_Block& block, blocks_ )
        if( block.first->Intersect2DWithLocalisation( localisation ) )
            return true;
    return false;
}
