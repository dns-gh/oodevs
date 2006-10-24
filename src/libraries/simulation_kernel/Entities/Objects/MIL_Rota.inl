//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Rota.inl $
// $Author: Nld $
// $Modtime: 26/04/05 14:16 $
// $Revision: 3 $
// $Workfile: MIL_Rota.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_Rota::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_Rota::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_Rota( type, nID, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::GetDanger
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
inline
uint MIL_Rota::GetDanger() const
{
    return nDanger_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Rota::GetNbcAgents
// Created: NLD 2004-10-29
// -----------------------------------------------------------------------------
inline
const MIL_Rota::T_NbcAgentSet& MIL_Rota::GetNbcAgents() const
{
    return nbcAgents_;
}
