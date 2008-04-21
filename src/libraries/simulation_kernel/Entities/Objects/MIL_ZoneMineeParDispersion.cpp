//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMineeParDispersion.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 7 $
// $Workfile: MIL_ZoneMineeParDispersion.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_ZoneMineeParDispersion.h"
#include "MIL_RealObjectType.h"
#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Knowledge/DEC_Knowledge_ObjectZoneMineeParDispersion.h"
#include "xeumeuleu/xml.h"



BOOST_CLASS_EXPORT_GUID( MIL_ZoneMineeParDispersion, "MIL_ZoneMineeParDispersion" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMineeParDispersion::MIL_ZoneMineeParDispersion( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle       ( type, nID, army )
    , rMinesDensity_     ( 0. )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_    ( 0 )    
{
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ZoneMineeParDispersion::MIL_ZoneMineeParDispersion()
    : MIL_Obstacle       ()
    , rMinesDensity_     ( 0. )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_    ( 0 )    
{
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMineeParDispersion::~MIL_ZoneMineeParDispersion()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_ZoneMineeParDispersion::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & nMinesActivityTime_
         & rMinesDensity_
         & nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::WriteSpecificAttributes
// Created: NLD 2007-02-06
// -----------------------------------------------------------------------------
void MIL_ZoneMineeParDispersion::WriteSpecificAttributes( xml::xostream& xos ) const
{
    xos << xml::content( "activity-time", nMinesActivityTime_ )
        << xml::content( "density", rMinesDensity_ );
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_ZoneMineeParDispersion::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx ) )
        return false;

    rMinesDensity_      = diaParameters[ nCurrentParamIdx++ ].ToFloat();
    nMinesActivityTime_ = (uint)diaParameters[ nCurrentParamIdx++ ].ToFloat();
    rSizeCoef_                       = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() );       // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = std::max( (uint)1, (uint)( rMinesDensity_ * rSizeCoef_ ) );  // Param DIA : densité de mines au m
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_ZoneMineeParDispersion::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );

    xis >> xml::optional() >> xml::content( "activity-time", nMinesActivityTime_ );

    if( nMinesActivityTime_ < 0 )
        xis.error( "nMinesActivityTime_ is not greater or equal to 0" );

    xis >> xml::optional() >> xml::content( "density", rMinesDensity_ );

    if( rMinesDensity_ < 0. )
        xis.error( "rMinesDensity_ is not greater or equal to 0." );

    rSizeCoef_                          = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = std::max( (uint)1, (uint)( rMinesDensity_ * rSizeCoef_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ZoneMineeParDispersion::Initialize( const ASN1T_MagicActionCreateObject& asn )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asn );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    if( !asn.m.specific_attributesPresent || asn.specific_attributes.t != T_ObjectAttributesSpecific_dispersed_mine_area )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    rMinesDensity_      = asn.specific_attributes.u.dispersed_mine_area->density;
    nMinesActivityTime_ = asn.specific_attributes.u.dispersed_mine_area->activity_time;

    rSizeCoef_                          = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_    = std::max( (uint)1, (uint)( rMinesDensity_ * rSizeCoef_ ) );
    return EnumObjectErrorCode::no_error;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::UpdateState
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void MIL_ZoneMineeParDispersion::UpdateState()
{
    MIL_RealObject_ABC::UpdateState();
    if( GetConstructionPercentage() >= 1. && nMinesActivityTime_ )
    {
        if( nDeathTimeStep_ == 0 )
            nDeathTimeStep_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + MIL_Tools::ConvertHoursToSim( nMinesActivityTime_ );
        if( nDeathTimeStep_ <= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
            MarkForDestruction();
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::Initialize
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void MIL_ZoneMineeParDispersion::Initialize( const TER_Localisation& localisation, uint nNbrMines )
{
    MIL_RealObject_ABC::Initialize( localisation );
    
    rSizeCoef_                       = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = nNbrMines;
    
    rMinesDensity_      = 0; //$$$ changer
    nMinesActivityTime_ = 0;
}

//=============================================================================
// EVENTS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::ProcessAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_ZoneMineeParDispersion::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentMovingInside( agent );
    if( !IsBypassed() && CanInteractWith( agent ) )
        ApplyAttrition( agent );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_ZoneMineeParDispersion::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    asnAttributes_.density       = rMinesDensity_;
    asnAttributes_.activity_time = nMinesActivityTime_;

    asnMsg().m.specific_attributesPresent              = 1;
    asnMsg().specific_attributes.t                     = T_ObjectAttributesSpecific_dispersed_mine_area;
    asnMsg().specific_attributes.u.dispersed_mine_area = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeParDispersion::CreateKnowledge
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_ZoneMineeParDispersion::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectZoneMineeParDispersion( teamKnowing, *this );
}
