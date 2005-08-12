//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Agent.inl $
// $Author: Age $
// $Modtime: 31/03/05 16:14 $
// $Revision: 16 $
// $Workfile: MOS_Agent.inl $
//
//*****************************************************************************

#include "MOS_Gtia.h"


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetCategory
/** @return 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
const std::string MOS_Agent::GetCategory() const
{
    return strCategory_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetAgentID
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_INLINE
MIL_AgentID MOS_Agent::GetAgentID() const
{
    return nAgentID_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetPosition
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
MOS_INLINE
const MT_Vector2D& MOS_Agent::GetPos() const
{
    return vPos_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetName 
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_INLINE
const std::string MOS_Agent::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::IsAutomate
// Created: FBD 03-01-06
//-----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsAutomate() const
{
    return bAutomate_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetParent
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Agent* MOS_Agent::GetParent() const
{
    return pParent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetSymbol
// Created: AGN 03-03-18
//-----------------------------------------------------------------------------
MOS_INLINE
const std::string& MOS_Agent::GetSymbol() const
{
    return symbolName_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetReports
/** @return 
*/
// Created: APE 2004-08-04
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Agent::T_ReportVector& MOS_Agent::GetReports()
{
    return reportVector_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsEmbraye
// Created: NLD 2003-10-20
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsEmbraye() const
{
    return bEmbraye_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetSpeed
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_INLINE
int MOS_Agent::GetSpeed() const
{
    return nSpeed_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetOldStance
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_INLINE
E_UnitPosture MOS_Agent::GetOldStance() const
{
    return nOldPosture_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetStance
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_INLINE
E_UnitPosture MOS_Agent::GetStance() const
{
    return nCurrentPosture_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetStanceCompletion
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_INLINE
int MOS_Agent::GetStanceCompletion() const
{
    return nPostureCompletionPourcentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsDead
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsDead() const
{
    return bDead_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsNeutralized
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsNeutralized() const
{
    return bNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetRawOpState
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_Agent::GetRawOpState() const
{
    return nRawOpState_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetRapFor
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_INLINE
E_ForceRatioState MOS_Agent::GetRapFor() const
{
    return nFightRateState_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsLoaded
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsLoaded() const
{
    return bLoadingState_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AreHumanTransportersReady
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::AreHumanTransportersReady() const
{
    return bHumanTransportersReady_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetComposition
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_AgentComposition& MOS_Agent::GetComposition() const
{
    return composition_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTeam
/** @return 
*/
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Team& MOS_Agent::GetTeam() const
{
    return GetGtia().GetTeam();
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetGtia
/** @return 
*/
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Gtia& MOS_Agent::GetGtia() const
{
    if( pGtia_ )
        return *pGtia_;

    if( pParent_ )
        return pParent_->GetGtia();

    assert( false );
    return *(MOS_Gtia*)0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsNBSSuitOn
/** @return 
*/
// Created: APE 2004-06-09
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsNBSSuitOn() const
{
    return bNbcProtectionSuitWorn_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetContaminatingNBCAgents
// Created: APE 2004-06-09
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Agent::T_NbcAgentVector& MOS_Agent::GetContaminatingNBCAgents() const
{
    return contaminatingNbcAgents_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetDirection
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
int MOS_Agent::GetDirection() const
{
    return nDirection_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetAltitude
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
int MOS_Agent::GetAltitude() const
{
    return nAltitude_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetROE
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
E_RulesOfEngagementState MOS_Agent::GetROE() const
{
    return nRulesOfEngagementState_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetCloseCombatState
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
E_CloseCombatState MOS_Agent::GetCloseCombatState() const
{
    return nCloseCombatState_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetReinforcements
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Agent::T_AgentIdVector& MOS_Agent::GetReinforcements() const
{
    return renforts_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetReinforced
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_Agent::GetReinforced() const
{
    return nPionRenforce_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTransportees
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Agent::T_AgentIdVector& MOS_Agent::GetTransportees() const
{
    return pionTransportes_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTransporter
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
MOS_INLINE
uint MOS_Agent::GetTransporter() const
{
    return nTransporteur_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsOutOfGas
/** @return 
*/
// Created: APE 2004-09-14
// -----------------------------------------------------------------------------
MOS_INLINE
bool MOS_Agent::IsOutOfGas() const
{
    return composition_.bEmptyGasTank_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetExperience
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Experience& MOS_Agent::GetExperience() const
{
    assert( pExperience_ );
    return *pExperience_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTiredness
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Tiredness& MOS_Agent::GetTiredness() const
{
    assert( pTiredness_ );
    return *pTiredness_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetMorale
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_Morale& MOS_Agent::GetMorale() const
{
    assert( pMorale_ );
    return *pMorale_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::ClearPath
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::ClearPath()
{
    pathFindVector_.clear();
    oldPathVector_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetName
/** @param  strName 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetName( const std::string& strName )
{
    sName_ = strName;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetAgentID
/** @param  nID 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetAgentID( MIL_AgentID nID )
{
    assert( nAgentID_ == 0 );
    nAgentID_ = nID;
    if( nMaxId_ < nAgentID_ )
        nMaxId_ = nAgentID_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetGtia
/** @param  gtia 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetGtia( MOS_Gtia& gtia )
{
    pGtia_ = &gtia;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetParent
/** @param  pParent 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetParent( MOS_Agent* pParent )
{
    pParent_ = pParent;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetCategory
/** @param  strCategory 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetCategory( const std::string& strCategory )
{
    strCategory_ = strCategory;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetSymbol
/** @param  strCategory 
*/
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetSymbol( const std::string& strSymbol )
{
    symbolName_ = strSymbol;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetPos
/** @param  vPos 
*/
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetPos( const MT_Vector2D& vPos )
{
    vPos_ = vPos;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetIsAutomate
/** @param  b 
*/
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetIsAutomate( bool b )
{
    bAutomate_ = b;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetEmbraye
/** @param  b 
*/
// Created: APE 2004-09-20
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::SetEmbraye( bool b )
{
    bEmbraye_ = b;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetContamination
/** @return 
*/
// Created: APE 2004-11-03
// -----------------------------------------------------------------------------
MOS_INLINE
int MOS_Agent::GetContamination() const
{
    return nContamination_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddChild
/** @param  agent 
*/
// Created: APE 2004-11-04
// -----------------------------------------------------------------------------
MOS_INLINE
void MOS_Agent::AddChild( MOS_Agent& agent )
{
    children_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetChildren
/** @return 
*/
// Created: APE 2004-11-04
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Agent::T_AgentVector& MOS_Agent::GetChildren()
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: mos_agent::GetTypePion
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_TypePion* MOS_Agent::GetTypePion() const
{
    return pTypePion_;
}

// -----------------------------------------------------------------------------
// Name: mos_agent::GetTypeAutomate
// Created: HME 2005-08-08
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_TypeAutomate* MOS_Agent::GetTypeAutomate() const
{
    return pTypeAutomate_;
}

