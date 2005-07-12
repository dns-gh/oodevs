// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.inl $
// $Author: Jvt $
// $Modtime: 24/03/05 14:03 $
// $Revision: 10 $
// $Workfile: MIL_Automate.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetLimas
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const T_LimaFlagedPtrMap& MIL_Automate::GetLimas() const
{
    return orderManager_.GetLimas();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetTC2
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG* MIL_Automate::GetTC2() const
{
    return pTC2_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsSurrendered
// Created: NLD 2005-02-24
// -----------------------------------------------------------------------------
inline
bool MIL_Automate::IsSurrendered() const
{
    return bSurrendered_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
bool MIL_Automate::IsPrisoner() const
{
    return bPrisoner_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPrisonerCamp
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
const MIL_CampPrisonniers* MIL_Automate::GetPrisonerCamp() const
{
    return pPrisonerCamp_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetRefugeeCamp
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
const MIL_CampRefugies* MIL_Automate::GetRefugeeCamp() const
{
    return pRefugeeCamp_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetID
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
uint MIL_Automate::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetType
// Created: NLD 2004-08-17
// -----------------------------------------------------------------------------
inline
const MIL_AutomateType& MIL_Automate::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetDecision
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
DEC_AutomateDecision& MIL_Automate::GetDecision() const
{
    assert( pDecision_ );
    return *pDecision_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPionPC
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& MIL_Automate::GetPionPC() const
{
    assert( pPionPC_ );
    return *pPionPC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
inline
MIL_KnowledgeGroup& MIL_Automate::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_AutomateOrderManager& MIL_Automate::GetOrderManager() const
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
MIL_AutomateOrderManager& MIL_Automate::GetOrderManager()
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetFuseau
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_Automate::GetFuseau() const
{
    return orderManager_.GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetDirDanger
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_Automate::GetDirDanger() const
{
    return orderManager_.GetDirDanger();
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetKSQuerier
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const DEC_KS_AutomateQuerier& MIL_Automate::GetKSQuerier() const
{
    return ksQuerier_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetPions
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const MIL_Automate::T_PionVector& MIL_Automate::GetPions() const
{
    return pions_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::GetName
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
const std::string& MIL_Automate::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::IsEmbraye
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
bool MIL_Automate::IsEmbraye() const
{
    return bEmbraye_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::HasAutomateModeChanged
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
inline
bool MIL_Automate::HasAutomateModeChanged() const
{
    return bAutomateModeChanged_;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Automate::RegisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
void MIL_Automate::RegisterPion( MIL_AgentPion& pion )
{
    assert( std::find( pions_.begin(), pions_.end(), &pion ) == pions_.end() );
    pions_.push_back( &pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::UnregisterPion
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
void MIL_Automate::UnregisterPion( MIL_AgentPion& pion )
{
    IT_PionVector it = std::find( pions_.begin(), pions_.end(), &pion );
    assert( it != pions_.end() );
    pions_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgAutomateOrder
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void MIL_Automate::OnReceiveMsgAutomateOrder( ASN1T_MsgAutomateOrder&   msg, MIL_MOSContextID nCtx )
{
    orderManager_.OnReceiveMsgAutomateOrder( msg, nCtx );
}
    
// -----------------------------------------------------------------------------
// Name: MIL_Automate::OnReceiveMsgOrderConduite
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void MIL_Automate::OnReceiveMsgOrderConduite( ASN1T_MsgOrderConduite&   msg, MIL_MOSContextID nCtx )
{
    assert( IsEmbraye() );
    orderManager_.OnReceiveMsgOrderConduite( msg, nCtx );
}


