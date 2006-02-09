//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Agent.inl $
// $Author: Age $
// $Modtime: 31/03/05 16:14 $
// $Revision: 16 $
// $Workfile: Agent.inl $
//
//*****************************************************************************

#include "Gtia.h"
#include "TypeAutomate.h"


// -----------------------------------------------------------------------------
// Name: Agent::GetCategory
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
const std::string Agent::GetCategory() const
{
    return strCategory_;
}

//-----------------------------------------------------------------------------
// Name: Agent::GetPosition
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& Agent::GetPos() const
{
    return vPos_;
}


//-----------------------------------------------------------------------------
// Name: Agent::GetName 
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
const std::string Agent::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: Agent::IsAutomate
// Created: FBD 03-01-06
//-----------------------------------------------------------------------------
inline
bool Agent::IsAutomate() const
{
    return bAutomate_;
}


//-----------------------------------------------------------------------------
// Name: Agent::GetParent
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
inline
Agent* Agent::GetParent() const
{
    return pParent_;
}


//-----------------------------------------------------------------------------
// Name: Agent::GetSymbol
// Created: AGN 03-03-18
//-----------------------------------------------------------------------------
inline
const std::string& Agent::GetSymbol() const
{
    return symbolName_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsEmbraye
// Created: NLD 2003-10-20
// -----------------------------------------------------------------------------
inline
bool Agent::IsEmbraye() const
{
    return bEmbraye_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetSpeed
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
inline
int Agent::GetSpeed() const
{
    return nSpeed_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetOldStance
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
inline
E_UnitPosture Agent::GetOldStance() const
{
    return nOldPosture_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetStance
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
inline
E_UnitPosture Agent::GetStance() const
{
    return nCurrentPosture_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetStanceCompletion
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
inline
int Agent::GetStanceCompletion() const
{
    return nPostureCompletionPourcentage_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsDead
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
bool Agent::IsDead() const
{
    return bDead_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsNeutralized
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
bool Agent::IsNeutralized() const
{
    return bNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetRawOpState
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
uint Agent::GetRawOpState() const
{
    return nRawOpState_;
}


// -----------------------------------------------------------------------------
// Name: Agent::IsLoaded
// Created: APE 2004-03-10
// -----------------------------------------------------------------------------
inline
bool Agent::IsLoaded() const
{
    return bLoadingState_;
}

// -----------------------------------------------------------------------------
// Name: Agent::AreHumanTransportersReady
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
bool Agent::AreHumanTransportersReady() const
{
    return bHumanTransportersReady_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetComposition
// Created: APE 2004-03-11
// -----------------------------------------------------------------------------
inline
const AgentComposition& Agent::GetComposition() const
{
    return composition_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetTeam
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
inline
Team& Agent::GetTeam() const
{
	return GetGtia().GetTeam();
}


// -----------------------------------------------------------------------------
// Name: Agent::GetGtia
// Created: APE 2004-04-29
// -----------------------------------------------------------------------------
inline
Gtia& Agent::GetGtia() const
{
    if( pGtia_ )
        return *pGtia_;

    if( pParent_ )
        return pParent_->GetGtia();

    assert( false );
    return *(Gtia*)0;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsNBSSuitOn
// Created: APE 2004-06-09
// -----------------------------------------------------------------------------
inline
bool Agent::IsNBSSuitOn() const
{
    return bNbcProtectionSuitWorn_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetContaminatingNBCAgents
// Created: APE 2004-06-09
// -----------------------------------------------------------------------------
inline
const Agent::T_NbcAgentVector& Agent::GetContaminatingNBCAgents() const
{
    return contaminatingNbcAgents_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetDirection
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
int Agent::GetDirection() const
{
    return nDirection_;
}


// -----------------------------------------------------------------------------
// Name: Agent::GetAltitude
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
int Agent::GetAltitude() const
{
    return nAltitude_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetReinforcements
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
const Agent::T_AgentIdVector& Agent::GetReinforcements() const
{
    return renforts_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetReinforced
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
inline
uint Agent::GetReinforced() const
{
    return nPionRenforce_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTransportees
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
inline
const Agent::T_AgentIdVector& Agent::GetTransportees() const
{
    return pionTransportes_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTransporter
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
inline
uint Agent::GetTransporter() const
{
    return nTransporteur_;
}

// -----------------------------------------------------------------------------
// Name: Agent::IsOutOfGas
// Created: APE 2004-09-14
// -----------------------------------------------------------------------------
inline
bool Agent::IsOutOfGas() const
{
    return composition_.bEmptyGasTank_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetExperience
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
const Experience& Agent::GetExperience() const
{
    assert( pExperience_ );
    return *pExperience_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTiredness
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
const Tiredness& Agent::GetTiredness() const
{
    assert( pTiredness_ );
    return *pTiredness_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetMorale
// Created: AGE 2005-03-22
// -----------------------------------------------------------------------------
inline
const Morale& Agent::GetMorale() const
{
    assert( pMorale_ );
    return *pMorale_;
}

// -----------------------------------------------------------------------------
// Name: Agent::ClearPath
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
inline
void Agent::ClearPath()
{
    pathFindVector_.clear();
    oldPathVector_.clear();
}


// -----------------------------------------------------------------------------
// Name: Agent::SetName
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void Agent::SetName( const std::string& strName )
{
    sName_ = strName;
}


// -----------------------------------------------------------------------------
// Name: Agent::SetAgentID
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void Agent::SetAgentID( MIL_AgentID nID )
{
    assert( nAgentID_ == 0 );
    nAgentID_ = nID;
    if( nMaxId_ < nAgentID_ )
        nMaxId_ = nAgentID_;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetGtia
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void Agent::SetGtia( Gtia& gtia )
{
    pGtia_ = &gtia;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetParent
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void Agent::SetParent( Agent* pParent )
{
    pParent_ = pParent;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetCategory
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void Agent::SetCategory( const std::string& strCategory )
{
    strCategory_ = strCategory;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetSymbol
// Created: APE 2004-09-03
// -----------------------------------------------------------------------------
inline
void Agent::SetSymbol( const std::string& strSymbol )
{
    symbolName_ = strSymbol;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetPos
// Created: APE 2004-08-26
// -----------------------------------------------------------------------------
inline
void Agent::SetPos( const MT_Vector2D& vPos )
{
    vPos_ = vPos;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetIsAutomate
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
inline
void Agent::SetIsAutomate( bool b )
{
    bAutomate_ = b;
}

// -----------------------------------------------------------------------------
// Name: Agent::SetEmbraye
// Created: APE 2004-09-20
// -----------------------------------------------------------------------------
inline
void Agent::SetEmbraye( bool b )
{
    bEmbraye_ = b;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetContamination
// Created: APE 2004-11-03
// -----------------------------------------------------------------------------
inline
int Agent::GetContamination() const
{
    return nContamination_;
}

// -----------------------------------------------------------------------------
// Name: Agent::AddChild
// Created: APE 2004-11-04
// -----------------------------------------------------------------------------
inline
void Agent::AddChild( Agent& agent )
{
    children_.push_back( &agent );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetChildren
// Created: APE 2004-11-04
// -----------------------------------------------------------------------------
inline
Agent::T_AgentVector& Agent::GetChildren()
{
    return children_;
}

// -----------------------------------------------------------------------------
// Name: mos_agent::GetTypePion
// Created: SBO 2005-08-03
// -----------------------------------------------------------------------------
inline
const TypePion* Agent::GetTypePion() const
{
    return pTypePion_;
}

// -----------------------------------------------------------------------------
// Name: mos_agent::GetTypeAutomate
// Created: HME 2005-08-08
// -----------------------------------------------------------------------------
inline
const TypeAutomate* Agent::GetTypeAutomate() const
{
    return pTypeAutomate_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetID
// Created: HME 2005-10-05
// -----------------------------------------------------------------------------
inline
const MIL_AgentID Agent::GetID() const
{
	return nAgentID_;
}

// -----------------------------------------------------------------------------
// Name: Agent::FindAgentKnowledge
// Created: HME 2005-10-06
// -----------------------------------------------------------------------------
inline
AgentKnowledge* Agent::FindAgentKnowledge( uint nId )
{
    return GetGtia().FindAgentKnowledge( nId );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsLogisticTC2
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
inline
bool Agent::IsLogisticTC2() const
{
    return IsAutomate() && !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG TC2" );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsLogisticBLD
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
inline
bool Agent::IsLogisticBLD() const
{
    return IsAutomate() && !strcmp( pTypeAutomate_->GetType().substr( 0, 16 ).c_str(), "Automate LOG BLD" );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsLogisticBLT
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
inline
bool Agent::IsLogisticBLT() const
{
    return IsAutomate() && !strcmp( pTypeAutomate_->GetType().substr( 0, 16 ).c_str(), "Automate LOG BLT" );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsLogisticMaintenance
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
inline
bool Agent::IsLogisticMaintenance() const
{
    return IsAutomate() &&
        (  !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG BLT Maintenance" )
        || !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG BLD Maintenance" ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsLogisticSante
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
inline
bool Agent::IsLogisticSante() const
{
    return IsAutomate() &&
        (  !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG BLT Sante" )
        || !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG BLD Sante" ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsLogisticRavitaillement
// Created: SBO 2005-10-27
// -----------------------------------------------------------------------------
inline
bool Agent::IsLogisticRavitaillement() const
{
    return IsAutomate() &&
        (  !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG BLT Ravitaillement" )
        || !strcmp( pTypeAutomate_->GetType().c_str(), "Automate LOG BLD Ravitaillement" ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::IsAggregated
// Created: HME 2005-11-30
// -----------------------------------------------------------------------------
inline
bool  Agent::IsAggregated() const
{
    return bAggregated_;
}