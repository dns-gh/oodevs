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
#include "protocol/ClientSenders.h"
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
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
MIL_LivingArea::MIL_LivingArea( xml::xistream& xis, unsigned long population )
{
    float totalArea = 0.f;
    xis >> xml::start( "living-area" )
            >> xml::list( "urban-block", *this, &MIL_LivingArea::ReadUrbanBlock, totalArea )
        >> xml::end;
    DistributeHumans( totalArea, population );
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

namespace
{
    template< typename T >
    bool Compare( const T& lhs, const T& rhs )
    {
        return lhs.first->GetFootprint()->ComputeArea() > rhs.first->GetFootprint()->ComputeArea();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LivingArea::DistributeHumans
// Created: LGY 2011-01-20
// -----------------------------------------------------------------------------
void MIL_LivingArea::DistributeHumans( float area, unsigned long population )
{
    std::sort( blocks_.begin(), blocks_.end(), boost::bind( &Compare< T_Block >, _1, _2 ) );
    unsigned long tmp = population;
    for( IT_Blocks it = blocks_.begin(); it != blocks_.end() && tmp > 0; ++it )
    {
        unsigned long person = static_cast< unsigned long >( it->first->GetFootprint()->ComputeArea() * population / area );
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
void MIL_LivingArea::SendFullState( client::PopulationUpdate& msg ) const
{
    BOOST_FOREACH( const T_Block& urbanBlock, blocks_ )
    {
        sword::PopulationUpdate_BlockOccupation& block = *msg().mutable_occupations()->Add();
        block.mutable_object()->set_id( MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *urbanBlock.first ).GetID() );
        block.set_number( urbanBlock.second );
    }
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
void MIL_LivingArea::StartMotivation( const std::string& /*motivation*/ )
{
    // NOTHING
}

