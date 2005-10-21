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
inline
const std::string MOS_Agent::GetCategory() const
{
    return strCategory_;
}

//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetPosition
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_Agent::GetPos() const
{
    return vPos_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetName 
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
const std::string MOS_Agent::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::IsAutomate
// Created: FBD 03-01-06
//-----------------------------------------------------------------------------
inline
bool MOS_Agent::IsAutomate() const
{
    return bAutomate_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetParent
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
inline
MOS_Agent* MOS_Agent::GetParent() const
{
    return pParent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetSymbol
// Created: AGN 03-03-18
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Agent::GetSymbol() const
{
    return symbolName_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsEmbraye
// Created: NLD 2003-10-20
// -----------------------------------------------------------------------------
inline
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
inline
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
inline
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
inline
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
inline
int MOS_Agent::GetStanceCompletion() const
{
    return nPostureCompletionPourcentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsDead
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsDead() const
{
    return bDead_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsNeutralized
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsNeutralized() const
{
    return bNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetRawOpState
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
uint MOS_Agent::GetRawOpState() const
{
    return nRawOpState_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsLoaded
/** @return 
*/
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsLoaded() const
{
    return bLoadingState_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AreHumanTransportersReady
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::AreHumanTransportersReady() const
{
    return bHumanTransportersReady_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetComposition
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
inline
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
inline
MOS_Team& MOS_Agent::GetTeam() const
{
	return GetGtia().GetTeam();
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetGtia
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
inline
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
// Created: APE 2004-06-09
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsNBSSuitOn() const
{
    return bNbcProtectionSuitWorn_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetContaminatingNBCAgents
// Created: APE 2004-06-09
// -----------------------------------------------------------------------------
inline
const MOS_Agent::T_NbcAgentVector& MOS_Agent::GetContaminatingNBCAgents() const
{
    return contaminatingNbcAgents_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetDirection
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
int MOS_Agent::GetDirection() const
{
    return nDirection_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetAltitude
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
int MOS_Agent::GetAltitude() const
{
    return nAltitude_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetReinforcements
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
const MOS_Agent::T_AgentIdVector& MOS_Agent::GetReinforcements() const
{
    return renforts_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetReinforced
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
inline
uint MOS_Agent::GetReinforced() const
{
    return nPionRenforce_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTransportees
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
inline
const MOS_Agent::T_AgentIdVector& MOS_Agent::GetTransportees() const
{
    return pionTransportes_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTransporter
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
inline
uint MOS_Agent::GetTransporter() const
{
    return nTransporteur_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsOutOfGas
// Created: APE 2004-09-14
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsOutOfGas() const
{
    return composition_.bEmptyGasTank_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetExperience
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
const MOS_Experience& MOS_Agent::GetExperience() const
{
    assert( pExperience_ );
    return *pExperience_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTiredness
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
const MOS_Tiredness& MOS_Agent::GetTiredness() const
{
    assert( pTiredness_ );
    return *pTiredness_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetMorale
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
const MOS_Morale& MOS_Agent::GetMorale() const
{
    assert( pMorale_ );
    return *pMorale_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::ClearPath
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
void MOS_Agent::ClearPath()
{
    pathFindVector_.clear();
    oldPathVector_.clear();
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetName
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetName( const std::string& strName )
{
    sName_ = strName;
}


// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetAgentID
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetAgentID( MIL_AgentID nID )
{
    assert( nAgentID_ == 0 );
    nAgentID_ = nID;
    if( nMaxId_ < nAgentID_ )
        nMaxId_ = nAgentID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetGtia
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetGtia( MOS_Gtia& gtia )
{
    pGtia_ = &gtia;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetParent
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetParent( MOS_Agent* pParent )
{
    pParent_ = pParent;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetCategory
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetCategory( const std::string& strCategory )
{
    strCategory_ = strCategory;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetSymbol
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetSymbol( const std::string& strSymbol )
{
    symbolName_ = strSymbol;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetPos
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetPos( const MT_Vector2D& vPos )
{
    vPos_ = vPos;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetIsAutomate
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetIsAutomate( bool b )
{
    bAutomate_ = b;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::SetEmbraye
// Created: APE 2004-09-20
// -----------------------------------------------------------------------------
inline
void MOS_Agent::SetEmbraye( bool b )
{
    bEmbraye_ = b;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetContamination
// Created: APE 2004-11-03
// -----------------------------------------------------------------------------
inline
int MOS_Agent::GetContamination() const
{
    return nContamination_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::AddChild
// Created: APE 2004-11-04
// -----------------------------------------------------------------------------
inline
void MOS_Agent::AddChild( MOS_Agent& agent )
{
    children_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetChildren
// Created: APE 2004-11-04
// -----------------------------------------------------------------------------
inline
MOS_Agent::T_AgentVector& MOS_Agent::GetChildren()
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: mos_agent::GetTypePion
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
const MOS_TypePion* MOS_Agent::GetTypePion() const
{
    return pTypePion_;
}

// -----------------------------------------------------------------------------
// Name: mos_agent::GetTypeAutomate
// Created: HME 2005-08-08
// -----------------------------------------------------------------------------
inline
const MOS_TypeAutomate* MOS_Agent::GetTypeAutomate() const
{
    return pTypeAutomate_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetID
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
inline
const MIL_AgentID MOS_Agent::GetID() const
{
	return nAgentID_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::FindAgentKnowledge
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
MOS_AgentKnowledge* MOS_Agent::FindAgentKnowledge( uint nId )
{
    return GetGtia().FindAgentKnowledge( nId );
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetFireResult
// Created: HME 2005-10-19
// -----------------------------------------------------------------------------
inline
const T_FireResults& MOS_Agent::GetFireResult() const
{
    return fireResults_;
}