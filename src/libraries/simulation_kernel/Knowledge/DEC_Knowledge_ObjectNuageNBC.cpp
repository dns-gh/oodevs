// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectNuageNBC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:35 $
// $Revision: 8 $
// $Workfile: DEC_Knowledge_ObjectNuageNBC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectNuageNBC.h"

#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_ObjectCollision.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_NuageNBC.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectNuageNBC, "DEC_Knowledge_ObjectNuageNBC" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectNuageNBC::DEC_Knowledge_ObjectNuageNBC( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , bSpecificAttributesValid_( false )
    , pNbcAgentType_           ( 0 )
{
    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectNuageNBC::DEC_Knowledge_ObjectNuageNBC()
    : DEC_Knowledge_Object     ()
    , bSpecificAttributesValid_( false )
    , pNbcAgentType_           ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectNuageNBC::~DEC_Knowledge_ObjectNuageNBC()
{
    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::load
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nTypeID;
    
    file >> boost::serialization::base_object< DEC_Knowledge_Object >( *this )
         >> nTypeID
         >> bSpecificAttributesValid_;

    pNbcAgentType_ = MIL_NbcAgentType::Find( nTypeID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::save
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    uint nbc = ( pNbcAgentType_ ? pNbcAgentType_->GetID() : (uint)-1 );
    file << boost::serialization::base_object< DEC_Knowledge_Object >( *this )
         << nbc
         << bSpecificAttributesValid_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_NuageNBC& nuageNBC = static_cast< const MIL_NuageNBC& >( *GetObjectKnown() );
        pNbcAgentType_ = &nuageNBC.GetNbcAgentType();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectNuageNBC::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectNuageNBC::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    assert( pNbcAgentType_ );   
    asnAttributes_.agent_nbc = pNbcAgentType_->GetID();

    asnMsg.m.attributs_specifiquesPresent    = 1;
    asnMsg.attributs_specifiques.t           = T_ObjectAttributesSpecific_nuage_nbc;
    asnMsg.attributs_specifiques.u.nuage_nbc = &asnAttributes_;
}
