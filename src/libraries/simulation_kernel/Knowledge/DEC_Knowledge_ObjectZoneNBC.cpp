// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectZoneNBC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 18:02 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_ObjectZoneNBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectZoneNBC.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_ZoneNBC.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectZoneNBC, "DEC_Knowledge_ObjectZoneNBC" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneNBC::DEC_Knowledge_ObjectZoneNBC( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , bSpecificAttributesValid_( false )
    , pNbcAgentType_           ( 0 )
{  
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneNBC::DEC_Knowledge_ObjectZoneNBC()
    : DEC_Knowledge_Object     ()
    , bSpecificAttributesValid_( false )
    , pNbcAgentType_           ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneNBC::~DEC_Knowledge_ObjectZoneNBC()
{   
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::load
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nID;
    
    file >> boost::serialization::base_object< DEC_Knowledge_Object >( *this )
         >> nID
         >> bSpecificAttributesValid_;
    pNbcAgentType_ = MIL_NbcAgentType::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned nbc = ( pNbcAgentType_ ? pNbcAgentType_->GetID() : (uint)-1 );
    file << boost::serialization::base_object< DEC_Knowledge_Object >( *this )
         << nbc
         << bSpecificAttributesValid_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_ZoneNBC& zoneNBC = static_cast< const MIL_ZoneNBC& >( *GetObjectKnown() );
        pNbcAgentType_ = &zoneNBC.GetNbcAgentType();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneNBC::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneNBC::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    assert( pNbcAgentType_ );   
    asnAttributes_.agent_nbc = pNbcAgentType_->GetID();

    asnMsg.m.attributs_specifiquesPresent   = 1;
    asnMsg.attributs_specifiques.t          = T_AttrObjectSpecific_zone_nbc;
    asnMsg.attributs_specifiques.u.zone_nbc = &asnAttributes_;
}
