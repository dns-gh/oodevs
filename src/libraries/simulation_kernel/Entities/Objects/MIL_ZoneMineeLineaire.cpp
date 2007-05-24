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
#include "Network/NET_ASN_Messages.h"
#include "Knowledge/DEC_Knowledge_ObjectZoneMineeLineaire.h"
#include "Tools/MIL_Tools.h"

BOOST_CLASS_EXPORT_GUID( MIL_ZoneMineeLineaire, "MIL_ZoneMineeLineaire" )

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMineeLineaire::MIL_ZoneMineeLineaire( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle       ( type, nID, army )
    , rMinesDensity_     ( 0. )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_    ( 0 )    
{
    // NOTHING    
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_ZoneMineeLineaire::MIL_ZoneMineeLineaire()
    : MIL_Obstacle       ()
    , rMinesDensity_     ( 0. )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_    ( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire destructor
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_ZoneMineeLineaire::~MIL_ZoneMineeLineaire()
{
    // NOTHING
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
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & nMinesActivityTime_
         & rMinesDensity_
         & nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::WriteSpecificAttributes
// Created: NLD 2007-02-06
// -----------------------------------------------------------------------------
void MIL_ZoneMineeLineaire::WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const
{
    archive.WriteField( "activity-time", nMinesActivityTime_ );
    archive.WriteField( "density"      , rMinesDensity_      );
}

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_ZoneMineeLineaire::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx ) )
        return false;

    rMinesDensity_      = diaParameters[ nCurrentParamIdx++ ].ToFloat();
    nMinesActivityTime_ = (uint)diaParameters[ nCurrentParamIdx++ ].ToFloat();

    rSizeCoef_                       = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = std::max( (uint)1, (uint)( rMinesDensity_ * rSizeCoef_ ) );    // Param DIA : densité de mines au m
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_ZoneMineeLineaire::Initialize( MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( archive );
    archive.ReadField( "activity-time", nMinesActivityTime_, CheckValueGreaterOrEqual( 0  ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );
    archive.ReadField( "density"      , rMinesDensity_     , CheckValueGreaterOrEqual( 0. ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );
    rSizeCoef_                       = MIL_Tools::ConvertSimToMeter(  GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = std::max( (uint)1, (uint)( rMinesDensity_ * rSizeCoef_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_ZoneMineeLineaire::Initialize( const ASN1T_MagicActionCreateObject& asn )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asn );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    if( !asn.m.attributs_specifiquesPresent || asn.attributs_specifiques.t != T_AttrObjectSpecific_zone_minee_lineaire )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    rMinesDensity_      = asn.attributs_specifiques.u.zone_minee_lineaire->densite;
    nMinesActivityTime_ = asn.attributs_specifiques.u.zone_minee_lineaire->delai_activite_mines;

    rSizeCoef_                       = MIL_Tools::ConvertSimToMeter( GetLocalisation().GetLength() ); // Coef      : tps construction/destruction au m
    nFullNbrDotationForConstruction_ = std::max( (uint)1, (uint)( rMinesDensity_ * rSizeCoef_ ) );
    return EnumObjectErrorCode::no_error;
}

//=============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::UpdateState
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void MIL_ZoneMineeLineaire::UpdateState()
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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_ZoneMineeLineaire::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    asnAttributes_.densite              = rMinesDensity_;
    asnAttributes_.delai_activite_mines = nMinesActivityTime_;

    asnMsg().m.attributs_specifiquesPresent = 1;
    asnMsg().attributs_specifiques.t                     = T_AttrObjectSpecific_zone_minee_lineaire;
    asnMsg().attributs_specifiques.u.zone_minee_lineaire = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_ZoneMineeLineaire::CreateKnowledge
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_ZoneMineeLineaire::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectZoneMineeLineaire( teamKnowing, *this );
}
