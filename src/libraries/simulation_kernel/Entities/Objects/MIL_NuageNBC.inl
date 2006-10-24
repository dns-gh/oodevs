//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_NuageNBC.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_NuageNBC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_NuageNBC::GetNbcAgentType
// Created: NLD 2004-05-04
// -----------------------------------------------------------------------------
inline
const MIL_NbcAgentType& MIL_NuageNBC::GetNbcAgentType() const
{
    assert( pNbcAgentType_ );
    return *pNbcAgentType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_NuageNBC::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_NuageNBC( type, nID, army );
}
