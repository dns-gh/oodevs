// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectZoneMineeParDispersion.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectZoneMineeParDispersion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectZoneMineeParDispersion.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_ZoneMineeParDispersion.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectZoneMineeParDispersion, "DEC_Knowledge_ObjectZoneMineeParDispersion" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneMineeParDispersion::DEC_Knowledge_ObjectZoneMineeParDispersion( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , nMinesActivityTime_      ( 0 )
    , rMinesDensity_           ( 0. )
    , bSpecificAttributesValid_( false )
{    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneMineeParDispersion::DEC_Knowledge_ObjectZoneMineeParDispersion()
    : DEC_Knowledge_Object     ()
    , nMinesActivityTime_      ( 0 )
    , rMinesDensity_           ( 0. )
    , bSpecificAttributesValid_( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneMineeParDispersion::~DEC_Knowledge_ObjectZoneMineeParDispersion()
{    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectZoneMineeParDispersion::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< DEC_Knowledge_Object >( *this );
    file & nMinesActivityTime_
         & rMinesDensity_
         & bSpecificAttributesValid_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() || GetArmy() != GetArmyKnowing() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_ZoneMineeParDispersion& object = static_cast< const MIL_ZoneMineeParDispersion& >( *GetObjectKnown() );

        nMinesActivityTime_ = object.GetMinesActivityTime();
        rMinesDensity_      = object.GetMinesDensity     ();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeParDispersion::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeParDispersion::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeParDispersion::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    asnAttributes_.activity_time = nMinesActivityTime_;
    asnAttributes_.density       = rMinesDensity_;

    asnMsg.m.specific_attributesPresent = 1;
    asnMsg.specific_attributes.t                     = T_ObjectAttributesSpecific_dispersed_mine_area;
    asnMsg.specific_attributes.u.dispersed_mine_area = &asnAttributes_;
}
