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
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/MotivationsVisitor_ABC.h>
#include <urban/PhysicalAttribute.h>
#include <boost/foreach.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LivingArea )

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea constructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::MIL_LivingArea()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea constructor
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
MIL_LivingArea::MIL_LivingArea( xml::xistream& xis, unsigned long population, unsigned int nID )
    : nID_       ( nID )
    , population_( population )
{
    float totalArea = 0.f;
    xis >> xml::start( "living-area" )
            >> xml::list( "urban-block", *this, &MIL_LivingArea::ReadUrbanBlock, totalArea )
        >> xml::end;
    DistributeHumans( totalArea );
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
// Name: MIL_LivingArea::ReadUrbanBlock
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::ReadUrbanBlock( xml::xistream& xis, float& area )
{
    const urban::TerrainObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetTerrainObject( xis.attribute< int >( "id" ) );
    if( !object )
        xis.error( "Error in loading living urban block of population" );
    area += object->GetFootprint()->ComputeArea();
    blocks_.push_back( T_Block( object , 0 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Compare
// Created: LGY 2011-01-26
// -----------------------------------------------------------------------------
bool MIL_LivingArea::Compare( const T_Block& lhs, const T_Block& rhs )
{

    return lhs.first->GetFootprint()->ComputeArea() * GetFloor( lhs ) * GetOccupation( lhs )
           > rhs.first->GetFootprint()->ComputeArea() * GetFloor( rhs ) * GetOccupation( rhs );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::DistributeHumans
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::DistributeHumans( float area )
{
    std::sort( blocks_.begin(), blocks_.end(), boost::bind( &MIL_LivingArea::Compare, this, _1, _2 ) );
    unsigned long tmp = population_;
    for( IT_Blocks it = blocks_.begin(); it != blocks_.end() && tmp > 0; ++it )
    {
        unsigned long person = static_cast< unsigned long >( it->first->GetFootprint()->ComputeArea() * population_ / area );
        if( tmp - person < 0 )
            person = tmp;
        it->second = person;
        tmp -= person;
    }
    if( tmp > 0 && !blocks_.empty() )
        blocks_.front().second += tmp;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::Register
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::Register( MIL_StructuralStateNotifier_ABC& structural )
{
    BOOST_FOREACH( const T_Block& block, blocks_ )
        MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *block.first ).Register( structural );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::load
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_LivingArea_ABC >( *this );
    file >> nID_
        >> population_;
    unsigned int size;
    file >> size;
    unsigned int blockId;
    unsigned int person;
    for( unsigned int i = 0; i < size; ++i )
    {
        file >> blockId
             >> person;
        const urban::TerrainObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetTerrainObject( blockId );
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
    file << nID_
         << population_;
    unsigned int size = blocks_.size();
    file << size;
    unsigned int id;
    BOOST_FOREACH( const T_Block& block, blocks_ )
    {
        id = block.first->GetId();
        file << id
             << block.second;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SendFullState
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::SendFullState() const
{
    client::PopulationUpdate msg;
    msg().mutable_id()->set_id( nID_ );
    BOOST_FOREACH( const T_Block& urbanBlock, blocks_ )
    {
        sword::PopulationUpdate_BlockOccupation& block = *msg().mutable_occupations()->Add();
        block.mutable_object()->set_id( MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *urbanBlock.first ).GetID() );
        block.set_number( urbanBlock.second );
    }
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::SendCreation
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::SendCreation( client::PopulationCreation& msg ) const
{
    BOOST_FOREACH( const T_Block& block, blocks_ )
        msg().add_objects()->set_id( MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *block.first ).GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::WriteODB
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::WriteODB( xml::xostream& xos ) const
{
    BOOST_FOREACH( const T_Block& block, blocks_ )
    {
        xos << xml::start( "urban-block" )
                << xml::attribute( "id", block.first->GetId() )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::HealthCount
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
float MIL_LivingArea::HealthCount() const
{
    float healthCount = 0;
    BOOST_FOREACH( const T_Block& block, blocks_ )
    {
        MIL_Object_ABC& object = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *block.first );
        if( object.Retrieve< MedicalCapacity >() )
        {
            const StructuralCapacity* structural = object.Retrieve< StructuralCapacity >();
            healthCount += structural ? 0.01f * structural->GetStructuralState() : 1.f;
        }
    }
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
        unsigned int part = static_cast< unsigned int >( population_ / blocks.size() );
        unsigned long tmp = population_;
        T_Identifiers identifiers;
        BOOST_FOREACH( const T_Block& block, blocks )
        {
            identifiers[ block.first->GetId() ] = part;
            tmp -= part;
        }
        if( tmp > 0 )
            identifiers.begin()->second += tmp;
       BOOST_FOREACH( T_Block& block, blocks_ )
        {
            CIT_Identifiers it = identifiers.find( block.first->GetId() );
            block.second = ( it == identifiers.end() ) ? 0u : it->second;
        }
        SendFullState();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetBlockUsage
// Created: LGY 2011-01-21
// -----------------------------------------------------------------------------
MIL_LivingArea::T_Blocks MIL_LivingArea::GetBlockUsage( const std::string& motivation ) const
{
    T_Blocks blocks;
    BOOST_FOREACH( const T_Block& block, blocks_ )
        if( HasUsage( block, motivation ) )
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
// Name: MIL_LivingArea::HasUsage
// Created: LGY 2011-01-21
// -----------------------------------------------------------------------------
bool MIL_LivingArea::HasUsage( const T_Block& block, const std::string& motivation ) const
{
    const urban::PhysicalAttribute* pPhysical = block.first->Retrieve< urban::PhysicalAttribute >();
    if( !pPhysical || !pPhysical->GetMotivations() )
        return false;
    std::map< std::string, float > motivations;
    MotivationsVisitor visitor( motivations );
    block.first->Accept( visitor );
    if( motivations.find( motivation ) == motivations.end() )
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetFloor
// Created: LGY 2011-01-26
// -----------------------------------------------------------------------------
unsigned int MIL_LivingArea::GetFloor( const T_Block& block ) const
{
    const urban::PhysicalAttribute* pPhysical = block.first->Retrieve< urban::PhysicalAttribute >();
    if( !pPhysical || !pPhysical->GetArchitecture() )
        return 1u;
    return pPhysical->GetArchitecture()->GetFloorNumber() + 1;
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::GetOccupation
// Created: LGY 2011-01-26
// -----------------------------------------------------------------------------
float MIL_LivingArea::GetOccupation( const T_Block& block ) const
{
    const urban::PhysicalAttribute* pPhysical = block.first->Retrieve< urban::PhysicalAttribute >();
    if( !pPhysical || !pPhysical->GetArchitecture() )
        return 1.f;
    return pPhysical->GetArchitecture()->GetOccupation();
}
