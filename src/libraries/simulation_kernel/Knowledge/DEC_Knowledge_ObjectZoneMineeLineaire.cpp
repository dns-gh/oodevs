// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectZoneMineeLineaire.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:11 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectZoneMineeLineaire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_ObjectZoneMineeLineaire.h"
#include "Entities/MIL_Army.h"
#include "Entities/Objects/MIL_ZoneMineeLineaire.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_ObjectZoneMineeLineaire, "DEC_Knowledge_ObjectZoneMineeLineaire" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire constructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneMineeLineaire::DEC_Knowledge_ObjectZoneMineeLineaire( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown )
    : DEC_Knowledge_Object     ( armyKnowing, objectKnown )
    , nMinesActivityTime_      ( 0 )
    , rMinesDensity_           ( 0. )
    , bSpecificAttributesValid_( false )
{    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire constructor
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneMineeLineaire::DEC_Knowledge_ObjectZoneMineeLineaire()
    : DEC_Knowledge_Object     ()
    , nMinesActivityTime_      ( 0 )
    , rMinesDensity_           ( 0. )
    , bSpecificAttributesValid_( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire destructor
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
DEC_Knowledge_ObjectZoneMineeLineaire::~DEC_Knowledge_ObjectZoneMineeLineaire()
{    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire::serialize
// Created: JVT 2005-03-25
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_ObjectZoneMineeLineaire::serialize( Archive& file, const uint )
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
// Name: DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes()
{
    if( !GetObjectKnown() || GetArmy() != GetArmyKnowing() )
        return;

    if( !bSpecificAttributesValid_ )
    {
        bSpecificAttributesValid_ = true;

        const MIL_ZoneMineeLineaire& object = static_cast< const MIL_ZoneMineeLineaire& >( *GetObjectKnown() );

        nMinesActivityTime_ = object.GetMinesActivityTime();
        rMinesDensity_      = object.GetMinesDensity     ();
        NotifyAttributeUpdated( eAttr_Specific );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes( const PHY_PerceptionLevel& /*currentPerceptionLevel*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& /*perception*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeLineaire::UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision& /*collision*/ )
{
    UpdateSpecificAttributes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_ObjectZoneMineeLineaire::BuildMsgSpecificAttributes
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
void DEC_Knowledge_ObjectZoneMineeLineaire::BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    if( !IsAttributeUpdated( eAttr_Specific ) || !bSpecificAttributesValid_ )
        return;

    asnAttributes_.delai_activite_mines = nMinesActivityTime_;
    asnAttributes_.densite              = rMinesDensity_;

    asnMsg.m.attributs_specifiquesPresent              = 1;
    asnMsg.attributs_specifiques.t                     = T_ObjectAttributesSpecific_zone_minee_lineaire;
    asnMsg.attributs_specifiques.u.zone_minee_lineaire = &asnAttributes_;
}
