//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Agent.inl $
// $Author: Nld $
// $Modtime: 14/02/05 11:58 $
// $Revision: 28 $
// $Workfile: MOS_Agent.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetAgentID
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
MIL_AgentID MOS_Agent::GetAgentID() const
{
    return nAgentID_;
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

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetAltitude
// Created: JVT 2004-09-27
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Agent::GetAltitude() const
{
    return nAltitude_;
}

//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
inline
const std::string MOS_Agent::GetName() const
{
    return sName_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetType
// Created: FBD 03-01-06
//-----------------------------------------------------------------------------
/*inline
E_AgentTypeID MOS_Agent::GetType() const
{
    return nType_;
}
*/

//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetArme
// Created: FBD 03-01-06
//-----------------------------------------------------------------------------
inline
E_TypeArme MOS_Agent::GetArme() const
{
    return nArme_;
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
// Name: MOS_Agent::GetItem
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
inline
QListViewItem* MOS_Agent::GetItem( MOS_AgentListView& listview ) const
{
    CIT_AgentItemMap itItem = agentItemMap_.find( &listview );
    if ( itItem != agentItemMap_.end() )
    {
        return itItem->second;
    }
    return 0;
}
//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetSpeed
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
inline
MT_Float MOS_Agent::GetSpeed() const
{
    return rSpeed_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetAttributeEditor
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
inline
MOS_AttrEditor* MOS_Agent::GetAttributeEditor() const
{
    return pAttrEditor_;
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: AGN 03-03-18
//-----------------------------------------------------------------------------
inline
const std::string& MOS_Agent::GetSymbol() const
{
    return symbolName_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::T_KnowledgeMap
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
/*inline
MOS_Agent::T_KnowledgeMap& MOS_Agent::GetAgentKnowledges()
{
    if( bAutomate_ )
        return knowledgeMap_;
    else
        return pParent_->GetAgentKnowledges();
}
*/

//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetTraceVector
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
inline
MOS_Agent::T_TraceVector& MOS_Agent::GetTraceVector()
{
    return TraceVector_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Agent::GetRCVector
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
inline
MOS_Agent::T_RCVector& MOS_Agent::GetRCVector()
{
    return RCVector_;
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
// Name: MOS_Agent::GetOldPosture
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
E_PostureType MOS_Agent::GetOldPosture() const
{
    return nOldPosture_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetCurrentPosture
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
E_PostureType MOS_Agent::GetCurrentPosture() const
{
    return nCurrentPosture_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetPostureCompletionPourcentage
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
inline
uint MOS_Agent::GetPostureCompletionPourcentage() const
{
    return nPostureCompletionPourcentage_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetExperience
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const MOS_Experience& MOS_Agent::GetExperience() const
{
    return *pExperience_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetTiredness
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const MOS_Tiredness& MOS_Agent::GetTiredness() const
{
    return *pTiredness_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetMorale
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
const MOS_Morale& MOS_Agent::GetMorale() const
{
    return *pMorale_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::GetElongationFactor
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
inline
MT_Float MOS_Agent::GetElongationFactor() const
{
    return rElongationFactor_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsMaintenancePionLog
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsMaintenancePionLog() const
{
    return bMaintenancePionLog_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsMedicalPionLog
// Created: NLD 2005-01-17
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsMedicalPionLog() const
{
    return bMedicalPionLog_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Agent::IsSupplyPionLog
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
inline
bool MOS_Agent::IsSupplyPionLog() const
{
    return bSupplyPionLog_;
}
