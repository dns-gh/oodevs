//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_Path.inl $
// $Author: Age $
// $Modtime: 13/04/05 18:34 $
// $Revision: 3 $
// $Workfile: DEC_Agent_Path.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const DEC_PathClass& DEC_Agent_Path::GetPathClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathType
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const DEC_PathType& DEC_Agent_Path::GetPathType() const
{
    return pathType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitSpeeds
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const PHY_Speeds& DEC_Agent_Path::GetUnitSpeeds() const
{
    return unitSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Agent_Path::GetUnitMaxSlope() const
{
    return rMaxSlope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetAutomataFuseau
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& DEC_Agent_Path::GetAutomataFuseau() const
{
    return automateFuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetFuseau
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& DEC_Agent_Path::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetDirDanger
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Agent_Path::GetDirDanger() const
{
    return vDirDanger_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgeObjects
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_Agent_Path::T_PathKnowledgeObjectVector& DEC_Agent_Path::GetPathKnowledgeObjects() const
{
    return pathKnowledgeObjectVector_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgeAgents
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_Agent_Path::T_PathKnowledgeAgentVector& DEC_Agent_Path::GetPathKnowledgeAgents() const
{
    return pathKnowledgeAgentVector_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::NeedRefine
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_Path::NeedRefine() const
{
    return bRefine_;
}


