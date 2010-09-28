// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectAttributeNBC.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectAttributeNBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectAttributeNBC.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/NBCAttribute.h"
#include "DEC_Knowledge_Object.h"
#include "protocol/protocol.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_ObjectAttributeNBC )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBC::DEC_Knowledge_ObjectAttributeNBC()
    : attr_       ( 0 )
    , dangerLevel_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBC::DEC_Knowledge_ObjectAttributeNBC( const NBCAttribute& attr )
    : attr_       ( &attr )
    , dangerLevel_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectAttributeNBC::~DEC_Knowledge_ObjectAttributeNBC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::load
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file >> const_cast< NBCAttribute*& >( attr_ );
    file >> dangerLevel_;
    unsigned int size;
    file >> size;
    for( IT_NBCAgentsList it = agents_.begin() ; it != agents_.end() ; )
        it = agents_.erase( it );
    int nID;
    for( unsigned int i = 0 ; i < size ; i++ )
    {
        file >> nID;
        agents_.push_back( nID );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ObjectAttribute_ABC >( *this );
    file << const_cast< NBCAttribute*& >( attr_ );
    file << dangerLevel_;
    unsigned int size = agents_.size();
    file << size;
    unsigned int nID;
    for( CIT_NBCAgentsList iter = agents_.begin() ; iter != agents_.end() ; ++iter )
    {
        nID = *iter;
        file << nID;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::Register
// Created: JCR 2008-08-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::Register( DEC_Knowledge_Object& knObject )
{
    knObject.AttachExtension( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateAttributes()
{
    if( !attr_ )
        return;
    dangerLevel_ = attr_->GetDangerLevel();
    for( IT_NBCAgentsList it = agents_.begin() ; it != agents_.end() ; )
        it = agents_.erase( it );
    for( NBCAttribute::CIT_NBCAgents iter = attr_->GetNBCAgents().begin() ; iter != attr_->GetNBCAgents().end() ; ++iter )
        agents_.push_back( ( *iter )->GetID() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateOnPerceptionLevel( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateOnPerception( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::UpdateOnCollision( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectAttributeNBC::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectAttributeNBC::Send( Common::ObjectAttributes& message ) const
{
    message.mutable_nbc()->set_danger_level( dangerLevel_ );
    message.mutable_nbc()->mutable_nbc_agents();
    if( attr_ )
        for( NBCAttribute::CIT_NBCAgents iter = attr_->GetNBCAgents().begin() ; iter != attr_->GetNBCAgents().end() ; ++iter )
        {
            Common::NBCAgentType& data = *message.mutable_nbc()->add_nbc_agents();
            data.set_id( ( *iter )->GetID() ) ;
        }
}
