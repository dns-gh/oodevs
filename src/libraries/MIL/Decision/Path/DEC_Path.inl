//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.inl $
// $Author: Age $
// $Modtime: 13/04/05 18:34 $
// $Revision: 3 $
// $Workfile: DEC_Path.inl $
//
//*****************************************************************************

 
//-----------------------------------------------------------------------------
// Name: DEC_Path::GetID
// Created: JDY 03-02-12
//-----------------------------------------------------------------------------    
inline
uint DEC_Path::GetID() const
{
    return nID_;
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::GetResult
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
inline
const DEC_Path::T_PathPointList& DEC_Path::GetResult() const
{
    assert( GetState() != eComputing );
    return resultList_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetUnitSpeeds
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const PHY_Speeds& DEC_Path::GetUnitSpeeds() const
{
    return unitSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetUnitMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Path::GetUnitMaxSlope() const
{
    return rMaxSlope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetFuseau
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& DEC_Path::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetDirDanger
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Path::GetDirDanger() const
{
    return vDirDanger_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetPathKnowledgeObjects
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_Path::T_PathKnowledgeObjectVector& DEC_Path::GetPathKnowledgeObjects() const
{
    return pathKnowledgeObjectVector_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetPathKnowledgeAgents
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_Path::T_PathKnowledgeAgentVector& DEC_Path::GetPathKnowledgeAgents() const
{
    return pathKnowledgeAgentVector_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::GetPathType
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_PathType& DEC_Path::GetPathType() const
{
    return pathType_;
}

//=============================================================================
// REFERENCES
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_Path::GetQueryMaker
// Created: JDY 03-03-11
//-----------------------------------------------------------------------------
inline
const MIL_AgentPion& DEC_Path::GetQueryMaker() const
{
    return queryMaker_;   
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::operator==
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
bool DEC_Path::operator==( const DEC_Path& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::operator!=
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
inline
bool DEC_Path::operator!=( const DEC_Path& rhs ) const
{
    return nID_ != rhs.nID_;
}


