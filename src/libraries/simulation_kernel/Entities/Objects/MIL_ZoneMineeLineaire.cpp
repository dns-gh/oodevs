//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMineeLineaire.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:29 $
// $Revision: 7 $
// $Workfile: MIL_ZoneMineeLineaire.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZoneMineeLineaire.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneMineeLineaire, "MIL_ZoneMineeLineaire" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMineeLineaire::MIL_ZoneMineeLineaire()
    : MIL_Obstacle( MIL_RealObjectType::zoneMineeLineaire_ )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMineeLineaire::~MIL_ZoneMineeLineaire()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive > 
void MIL_ZoneMineeLineaire::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_ZoneMineeLineaire::Initialize( const MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
        return false;

    rSizeCoef_                          = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_    = (uint)( diaParameters[ nCurrentParamIdx ].ToFloat() * rSizeCoef_ );                     // Param DIA : densité de mines au m
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_ZoneMineeLineaire::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );
    rSizeCoef_                       = MIL_Tools::ConvertSimToMeter(  GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = (uint)rSizeCoef_;                                                              
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ZoneMineeLineaire::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asn )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( nID, asn );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    rSizeCoef_                          = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_    = (uint)rSizeCoef_;                                                              
    return EnumObjectErrorCode::no_error;
}

//=============================================================================
// EVENTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::ProcessAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_ZoneMineeLineaire::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentMovingInside( agent );
    if( !IsBypassed() && CanInteractWith( agent ) )
        ApplyAttrition( agent );
}
