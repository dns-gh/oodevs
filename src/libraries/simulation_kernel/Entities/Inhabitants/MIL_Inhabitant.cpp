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
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/MIL_Formation.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/MIL_IDManager.h"
#include "tools/MIL_Tools.h"
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/vector.hpp>

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
    // $$$$ _RC_ JSR 2010-12-06: commenté temporairement pour réparer le build
    /*
    unsigned int nTypeID = population->GetType().GetID();
    archive << nTypeID;*/
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Inhabitant* population, const unsigned int /*version*/ )
{
    // $$$$ _RC_ JSR 2010-12-06: commenté temporairement pour réparer le build
    /*unsigned int nTypeID;
    archive >> nTypeID;
    const MIL_InhabitantType* pType = MIL_InhabitantType::Find( nTypeID );
    //assert( pType );
    if( pType )
        ::new( population )MIL_Inhabitant( *pType);*/
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( xml::xistream& xis, const MIL_InhabitantType& type, MIL_Army_ABC& army )
    : MIL_Entity_ABC          ( xis )
    , pType_                  ( &type )
    , nID_                    ( xis.attribute< unsigned int >( "id" ) )
    , pArmy_                  ( &army )
    , healthy_                ( 0 )
    , wounded_                ( 0 )
    , dead_                   ( 0 )
{
    idManager_.Lock( nID_ );
    xis >> xml::start( "composition" )
        >> xml::attribute( "healthy", healthy_ )
        >> xml::attribute( "wounded", wounded_ )
        >> xml::attribute( "dead", dead_ )
        >> xml::end
        >> xml::start( "living-area" )
        >> xml::list( "urban-block", *this, &MIL_Inhabitant::ReadUrbanBlock )
        >> xml::end;

    xis >> xml::content( "information", text_ );
    xis >> xml::optional >> xml::start( "extensions" )
        >> xml::list( "entry", *this, &MIL_Inhabitant::ReadExtension )
        >> xml::end;
    pArmy_->RegisterInhabitant( *this );    
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant(const MIL_InhabitantType& type )
    : MIL_Entity_ABC          ( type.GetName() )
    , pType_                  ( &type )
    , nID_                    ( 0 )
    , pArmy_                  ( 0 )
    , healthy_                ( 0 )
    , wounded_                ( 0 )
    , dead_                   ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant constructor
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant::MIL_Inhabitant( const MIL_InhabitantType& type, MIL_Army_ABC& army, const MT_Vector2D& point, int number, const std::string& name )
    : MIL_Entity_ABC          ( name )
    , pType_                  ( &type )
    , nID_                    ( idManager_.GetFreeId() )
    , pArmy_                  ( &army )
    , healthy_                ( 0 )
    , wounded_                ( 0 )
    , dead_                   ( 0 )
{
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

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::load
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::load( MIL_CheckPointInArchive& file, const unsigned int )
{
     // $$$$ _RC_ JSR 2010-12-06: commenté temporairement pour réparer le build
    /*file >> boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    idManager_.Lock( nID_ );*/
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::save
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    // $$$$ _RC_ JSR 2010-12-06: commenté temporairement pour réparer le build
    /*file << boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file << nID_
         << pArmy_;      */
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
            << xml::attribute( "healthy", healthy_ )
            << xml::attribute( "wounded", wounded_ )
            << xml::attribute( "dead", dead_ )
        << xml::end
        << xml::content( "information", text_ )
        << xml::start( "living-area" );
        for( CIT_livingUrbanBlockVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
        {
            xos << xml::start( "urban-block" )
                << xml::attribute( "id", ( *it )->GetId() )
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
void MIL_Inhabitant::ReadUrbanBlock( xml::xistream& xis )
{
    int id; 
    xis >> xml::attribute( "id", id );
    const urban::TerrainObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetTerrainObject( id );
    if( !object )
        xis.error( "Error in loading living urban block of population " + GetName() );
    livingUrbanObject_.push_back( object );

}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendCreation
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendCreation() const
{
    client::PopulationCreation asnMsg;
    asnMsg().mutable_id()->set_id( nID_ );
    asnMsg().mutable_type()->set_id( pType_->GetID() );
    asnMsg().mutable_party()->set_id( pArmy_->GetID() );
    asnMsg().set_text( text_ );
    asnMsg().set_name( GetName() );
    for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        MsgsSimToClient::Extension_Entry* entry = asnMsg().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
    for( CIT_livingUrbanBlockVector it = livingUrbanObject_.begin(); it != livingUrbanObject_.end(); ++it )
    {
        Common::UrbanObjectId* blockId = asnMsg().add_blocks();
        blockId->set_id( ( *it )->GetId() );
    }
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::SendFullState
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::SendFullState() const
{
    client::PopulationUpdate asnMsg;
    asnMsg().mutable_id()->set_id( nID_ );
    asnMsg().set_healthy( healthy_ );
    asnMsg().set_dead( dead_ );
    asnMsg().set_wounded( wounded_ );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Inhabitant::UpdateNetwork
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void MIL_Inhabitant::UpdateNetwork()
{

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
