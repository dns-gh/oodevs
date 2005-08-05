//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_CampRefugies.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 6 $
// $Workfile: MIL_CampRefugies.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_CampRefugies.h"
#include "MIL_RealObjectType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( MIL_CampRefugies, "MIL_CampRefugies" )

//-----------------------------------------------------------------------------
// Name: MIL_CampRefugies constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampRefugies::MIL_CampRefugies()
    : MIL_RealObject_ABC( MIL_RealObjectType::campRefugies_ )
    , pTC2_             ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MIL_CampRefugies destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_CampRefugies::~MIL_CampRefugies()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_CampRefugies::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this )
         & pTC2_;

}


// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
bool MIL_CampRefugies::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
        return false;

    DEC_AutomateDecision* pDecTC2 = diaParameters[ nCurrentParamIdx ].ToUserObject( pDecTC2 );
    if( !pDecTC2 )
        return false;

    if( !pDecTC2->GetAutomate().GetType().IsLogistic() )
        return false;

    pTC2_ = static_cast< MIL_AutomateLOG* >( &pDecTC2->GetAutomate() );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
void MIL_CampRefugies::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );

    uint nTC2_;
    archive.ReadField( "TC2", nTC2_ );

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nTC2_ );
        if( !pTC2Tmp )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is invalid", archive.GetContext() );
    if( !pTC2Tmp->GetType().IsLogistic() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate TC2 specified is not a logistic automate", archive.GetContext() );
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Initialize
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_CampRefugies::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    if( !asnCreateObject.m.attributs_specifiquesPresent || asnCreateObject.attributs_specifiques.t != T_AttrObjectSpecific_camp_refugies )
        return EnumObjectErrorCode::error_missing_specific_attributes;

    MIL_Automate* pTC2Tmp = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( asnCreateObject.attributs_specifiques.u.camp_refugies->tc2 );
    if( !pTC2Tmp || !pTC2Tmp->GetType().IsLogistic() )
        return EnumObjectErrorCode::error_invalid_specific_attributes;
    pTC2_ = static_cast< MIL_AutomateLOG* >( pTC2Tmp );

    return MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::ProcessAgentInside
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_CampRefugies::ProcessAgentInside( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentInside( agent );
    if( pTC2_ )
        agent.GetRole< PHY_RoleInterface_Refugee >().NotifyInsideRefugeeCamp( *this );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::ProcessAgentEntering
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_CampRefugies::ProcessAgentEntering( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentEntering( agent );
    if( pTC2_ )
        agent.GetRole< PHY_RoleInterface_Refugee >().NotifyInsideRefugeeCamp( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::ProcessAgentExiting
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
void MIL_CampRefugies::ProcessAgentExiting( MIL_Agent_ABC& agent )
{
    MIL_RealObject_ABC::ProcessAgentExiting( agent );
    if( pTC2_ )
        agent.GetRole< PHY_RoleInterface_Refugee >().NotifyOutsideRefugeeCamp( *this );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::WriteSpecificAttributes
// Created: NLD 2003-11-25
// -----------------------------------------------------------------------------
void MIL_CampRefugies::WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg )
{
    assert( pTC2_ );

    asnAttributes_.tc2 = pTC2_->GetID();

    asnMsg.GetAsnMsg().m.attributs_specifiquesPresent        = 1;
    asnMsg.GetAsnMsg().attributs_specifiques.t               = T_AttrObjectSpecific_camp_refugies;
    asnMsg.GetAsnMsg().attributs_specifiques.u.camp_refugies = &asnAttributes_;
}