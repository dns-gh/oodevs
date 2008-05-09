// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentPion.inl $
// $Author: Jvt $
// $Modtime: 24/03/05 15:24 $
// $Revision: 5 $
// $Workfile: MIL_AgentPion.inl $
//
// *****************************************************************************

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetKnowledge
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
inline
DEC_KnowledgeBlackBoard_AgentPion& MIL_AgentPion::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAutomate
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
const MIL_Automate& MIL_AgentPion::GetAutomate() const
{
    assert( pAutomate_ );
    return *pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetAutomate
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
inline
MIL_Automate& MIL_AgentPion::GetAutomate()
{
    assert( pAutomate_ );
    return *pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetType
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
const MIL_AgentTypePion& MIL_AgentPion::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::IsPC
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
bool MIL_AgentPion::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetOrderManager
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
MIL_PionOrderManager& MIL_AgentPion::GetOrderManager()
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetOrderManager
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_PionOrderManager& MIL_AgentPion::GetOrderManager() const
{
    return orderManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetDirDanger
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_AgentPion::GetDirDanger() const
{
    return orderManager_.GetDirDanger();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetFuseau
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_AgentPion::GetFuseau() const
{
    return orderManager_.GetFuseau();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::GetLimas
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
inline
const T_LimaVector& MIL_AgentPion::GetLimas() const
{
    return orderManager_.GetLimas();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_AgentPion::FindLima( const MIL_LimaFunction& function ) const
{
    return orderManager_.FindLima( function );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_AgentPion::FindLima( uint nID ) const
{
    return orderManager_.FindLima( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::FindLima
// Created: NLD 2007-04-30
// -----------------------------------------------------------------------------
inline
MIL_LimaOrder* MIL_AgentPion::FindNextScheduledLima() const
{
    return orderManager_.FindNextScheduledLima();
}

