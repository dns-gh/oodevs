//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_BouchonMines.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 7 $
// $Workfile: MIL_BouchonMines.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_BouchonMines.h"
#include "MIL_RealObjectType.h"
#include "Network/NET_ASN_Messages.h"
#include "Knowledge/DEC_Knowledge_ObjectBouchonMines.h"
#include "Tools/MIL_Tools.h"
#include <xeumeuleu/xml.h>

BOOST_CLASS_EXPORT_GUID( MIL_BouchonMines, "MIL_BouchonMines" )

//-----------------------------------------------------------------------------
// Name: MIL_BouchonMines constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_BouchonMines::MIL_BouchonMines( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
    : MIL_Obstacle       ( type, nID, army )
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_    ( 0 )    
{
    // NOTHING    
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines constructor
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_BouchonMines::MIL_BouchonMines()
    : MIL_Obstacle       ()
    , nMinesActivityTime_( 0 )
    , nDeathTimeStep_    ( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_BouchonMines destructor
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_BouchonMines::~MIL_BouchonMines()
{
    // NOTHING
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive > 
void MIL_BouchonMines::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & nMinesActivityTime_
         & nDeathTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::WriteSpecificAttributes
// Created: NLD 2007-02-06
// -----------------------------------------------------------------------------
void MIL_BouchonMines::WriteSpecificAttributes( xml::xostream& xos ) const
{
    xos << xml::content( "activity-time", nMinesActivityTime_ );
 }

//=============================================================================
// INIT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
bool MIL_BouchonMines::Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( obstacleType, diaParameters, nCurrentParamIdx ) )
        return false;

    nMinesActivityTime_ = (uint)diaParameters[ nCurrentParamIdx++ ].ToFloat();
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
void MIL_BouchonMines::Initialize( xml::xistream& xis )
{
    MIL_RealObject_ABC::Initialize( xis );
    xis >> xml::optional() >> xml::content( "activity-time", nMinesActivityTime_ );

    if( nMinesActivityTime_ < 0 )
        xis.error( "nMinesActivityTime_ is not greater or equal than 0" );
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Initialize
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_BouchonMines::Initialize( const ASN1T_MagicActionCreateObject& asn )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( asn );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    if( !asn.m.specific_attributesPresent || asn.specific_attributes.t != T_ObjectAttributesSpecific_mine_jam )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    nMinesActivityTime_ = asn.specific_attributes.u.mine_jam->activity_time;
    return EnumObjectErrorCode::no_error;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::UpdateState
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
void MIL_BouchonMines::UpdateState()
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
// Name: MIL_BouchonMines::ProcessAgentMovingInside
// Created: NLD 2004-04-29
// -----------------------------------------------------------------------------
void MIL_BouchonMines::ProcessAgentMovingInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentMovingInside( agent );
    if( !IsBypassed() && CanInteractWith( agent ) )
        ApplyAttrition( agent );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_BouchonMines::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    asnAttributes_.activity_time = nMinesActivityTime_;

    asnMsg().m.specific_attributesPresent   = 1;
    asnMsg().specific_attributes.t          = T_ObjectAttributesSpecific_mine_jam;
    asnMsg().specific_attributes.u.mine_jam = &asnAttributes_;
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::CreateKnowledge
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
DEC_Knowledge_Object& MIL_BouchonMines::CreateKnowledge( const MIL_Army& teamKnowing )
{
    return *new DEC_Knowledge_ObjectBouchonMines( teamKnowing, *this );
}
