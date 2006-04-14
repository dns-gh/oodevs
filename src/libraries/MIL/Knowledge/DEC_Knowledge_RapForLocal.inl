// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapForLocal.inl $
// $Author: Nld $
// $Modtime: 1/09/04 11:31 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_RapForLocal.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal::GetDangerousEnemies
// Created: NLD 2006-01-24
// -----------------------------------------------------------------------------
inline
const T_KnowledgeAgentDiaIDVector& DEC_Knowledge_RapForLocal::GetDangerousEnemies() const
{
    return dangerousEnemies_;
}
