// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Inhabitant.h"
#include "MIL_InhabitantType.h"
#include "MIL_AgentServer.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Objects/MedicalCapacity.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/MIL_Army_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/MIL_IDManager.h"
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Inhabitant )

namespace
{
    MIL_IDManager idManager_;
    template< typename R >
    void SaveRole( const MIL_Inhabitant& population, MIL_CheckPointOutArchive& file )
    {
        const R* const role = &population.GetRole< R >();
        file << role;
    }
}

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID = population->GetType().GetID();
    archive << nTypeID;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    archive >> nTypeID;
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( nTypeID );
    if( pType )
        ::new( population )MIL_Inhabitant( *pType);
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( xml::xistream& xis, const MIL_InhabitantType& type, MIL_Army_ABC& army )
    : MIL_Entity_ABC( xis )
    , pType_             ( &type )
    , nID_               ( xis.attribute< unsigned int >( "id" ) )
    , pArmy_             ( &army )
    , nNbrHealthyHumans_ ( 0 )
    , nNbrDeadHumans_    ( 0 )
    , nNbrWoundedHumans_ ( 0 )
    , healthNeed_        ( 0 )
    , healthSatisfaction_( 0 )
    , healthChanged_     ( false )
{
    float totalArea = 0.f;
    idManager_.Lock( nID_ );
    xis >> xml::start( "composition" )
            >> xml::attribute( "healthy", nNbrHealthyHumans_ )
            >> xml::attribute( "wounded", nNbrWoundedHumans_ )
            >> xml::attribute( "dead", nNbrDeadHumans_ )
        >> xml::end
        >> xml::start( "living-area" )
            >> xml::list( "urban-block", *this, &MIL_Inhabitant::ReadUrbanBlock, totalArea )
        >> xml::end
        >> xml::start( "information" )
            >> xml::optional >> text_
        >> xml::end
        >> xml::start( "health-need" )
            >> xml::attribute( "quantity", healthNeed_ )
        >> xml::end
        >> xml::optional
        >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &MIL_Inhabitant::ReadExtension )
        >> xml::end;
    DistributeHumans( totalArea );
    ComputeHealthSatisfaction();
    pArmy_->RegisterInhabitant( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant(const MIL_InhabitantType& type )
    : MIL_Entity_ABC( type.GetName() )
    , pType_             ( &type )
    , nID_               ( 0 )
    , pArmy_             ( 0 )
    , nNbrHealthyHumans_ ( 0 )
    , nNbrDeadHumans_    ( 0 )
    , nNbrWoundedHumans_ ( 0 )
    , healthNeed_        ( 0 )
    , healthSatisfaction_( 0 )
    , healthChanged_     ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant destructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::~MIL_Inhabitant()
{
    if( pArmy_ )
        pArmy_->UnregisterInhabitant( *this );
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
// Name: MIL_Inhabitant::DistributeHumans
// Created: LGY 2010-12-28
// -----------------------------------------------------------------------------
void MIL_Inhabitant::DistributeHumans( float area )
{
    std::sort( urbanBlocks_.begin(), urbanBlocks_.end(), boost::bind( &Compare< T_UrbanBlock >, _1, _2 ) );
    unsigned long population = nNbrHealthyHumans_ + nNbrWoundedHumans_ + nNbrDeadHumans_;
    unsigned long tmp = population;
    for( IT_UrbanBlocks it = urbanBlocks_.begin(); it != urbanBlocks_.end() && tmp > 0; ++it )
    {
        unsigned long person = static_cast< unsigned long >( it->first->GetFootprint()->ComputeArea() * population / area );
        if( tmp - person < 0 )
            person = tmp;
        it->second = person;
        tmp -= person;
    }
    if( tmp > 0 && !urbanBlocks_.empty() )
        urbanBlocks_.front().second += tmp;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::load
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    idManager_.Lock( nID_ );
    file >> text_
         >> nNbrHealthyHumans_
         >> nNbrDeadHumans_
         >> nNbrWoundedHumans_
         >> healthNeed_
         >> healthSatisfaction_;
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
            urbanBlocks_.push_back( T_UrbanBlock( object , person ) );
    }
    file >> size;
    std::string first;
    std::string second;
    for( unsigned int i = 0; i < size; ++i )
    {
        file >> first
             >> second;
        extensions_[ first ] = second;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::save
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file << nID_
         << pArmy_
         << text_
         << nNbrHealthyHumans_
         << nNbrDeadHumans_
         << nNbrWoundedHumans_
         << healthNeed_
         << healthSatisfaction_;
    unsigned int size = urbanBlocks_.size();
    file << size;
    unsigned int blockId;
    for( CIT_UrbanBlocks it = urbanBlocks_.begin(); it != urbanBlocks_.end(); ++it )
    {
        blockId = it->first->GetId();
        file << blockId
             << it->second;
    }
    size = extensions_.size();
    file << size;
    for( CIT_Extensions it = extensions_.begin(); it != extensions_.end(); ++it )
        file << it->first
             << it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::WriteODB
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    assert( pArmy_ );
    xos << xml::start( "inhabitant" );
    MIL_Entity_ABC::WriteODB ( xos ) ;
    xos << xml::attribute( "id", nID_ )
        << xml::attribute( "type", pType_->GetName() )
        << xml::start( "composition" )
            << xml::attribute( "healthy", nNbrHealthyHumans_ )
            << xml::attribute( "wounded", nNbrWoundedHumans_ )
            << xml::attribute( "dead", nNbrDeadHumans_ )
        << xml::end
        << xml::start( "health-need" )
            << xml::attribute( "quantity", healthNeed_ )
        << xml::end
        << xml::content( "information", text_ )
        << xml::start( "living-area" );
            for( CIT_UrbanBlocks it = urbanBlocks_.begin(); it != urbanBlocks_.end(); ++it )
            {
                xos << xml::start( "urban-block" )
                        << xml::attribute( "id", it->first->GetId() )
                    << xml::end;
            }
        xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ReadExtension
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key" ) ] = xis.attribute< std::string >( "value" );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ReadUrbanBlock
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::ReadUrbanBlock( xml::xistream& xis, float& area )
{
    const urban::TerrainObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetTerrainObject( xis.attribute< int >( "id" ) );
    if( !object )
        xis.error( "Error in loading living urban block of population " + GetName() );
    area += object->GetFootprint()->ComputeArea();
    urbanBlocks_.push_back( T_UrbanBlock( object , 0 ) );
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *object ).Register( *static_cast< MIL_StructuralStateNotifier_ABC* >( this ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendCreation
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendCreation() const
{
    client::PopulationCreation msg;
    msg().mutable_id()->set_id( nID_ );
    msg().mutable_type()->set_id( pType_->GetID() );
    msg().mutable_party()->set_id( pArmy_->GetID() );
    msg().set_text( text_ );
    msg().set_name( GetName() );
    BOOST_FOREACH( const T_Extensions::value_type& extension, extensions_ )
    {
        sword::Extension_Entry* entry = msg().mutable_extension()->add_entries();
        entry->set_name( extension.first );
        entry->set_value( extension.second );
    }
    BOOST_FOREACH( const T_UrbanBlock& block, urbanBlocks_ )
    {
        msg().add_objects()->set_id( MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *block.first ).GetID() );
    }
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendFullState
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendFullState() const
{
    client::PopulationUpdate msg;
    msg().mutable_id()->set_id( nID_ );
    msg().set_healthy( nNbrHealthyHumans_ );
    msg().set_dead( nNbrDeadHumans_ );
    msg().set_wounded( nNbrWoundedHumans_ );
    msg().mutable_satisfaction()->set_health( healthSatisfaction_ );
    BOOST_FOREACH( const T_UrbanBlock& urbanBlock, urbanBlocks_ )
    {
        sword::PopulationUpdate_BlockOccupation& block = *msg().mutable_occupations()->Add();
        block.mutable_object()->set_id( MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *urbanBlock.first ).GetID() );
        block.set_number( urbanBlock.second );
    }
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateState
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateNetwork
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateNetwork()
{
    if( healthChanged_ )
    {
        client::PopulationUpdate msg;
        msg().mutable_id()->set_id( nID_ );
        msg().mutable_satisfaction()->set_health( healthSatisfaction_ );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
    healthChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetID
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
unsigned int MIL_Inhabitant::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetType
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
const MIL_InhabitantType& MIL_Inhabitant::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::GetArmy
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_Inhabitant::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::NotifyStructuralStateChanged
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::NotifyStructuralStateChanged( unsigned int /*structuralState*/, const MIL_Object_ABC& object )
{
    if( object.Retrieve< MedicalCapacity >() )
        ComputeHealthSatisfaction();
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::ComputeHealthSatisfaction
// Created: JSR 2011-01-14
// -----------------------------------------------------------------------------
void MIL_Inhabitant::ComputeHealthSatisfaction()
{
    float healthSatisfaction;
    if( healthNeed_ == 0 )
        healthSatisfaction = 1.f;
    else
    {
        float healthCount = 0;
        BOOST_FOREACH( const T_UrbanBlock& urbanBlock, urbanBlocks_ )
        {
            MIL_Object_ABC& object = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *urbanBlock.first );
            if( object.Retrieve< MedicalCapacity >() )
            {
                const StructuralCapacity* structural = object.Retrieve< StructuralCapacity >();
                healthCount += structural ? 0.01f * structural->GetStructuralState() : 1.f;
            }
        }
        healthSatisfaction = std::min( 1.f, healthCount / healthNeed_ );
    }
    if( healthSatisfaction != healthSatisfaction_ )
    {
        healthChanged_ = true;
        healthSatisfaction_ = healthSatisfaction;
    }
}
