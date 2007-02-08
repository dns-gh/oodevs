//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_BouchonMines.inl $
// $Author: Nld $
// $Modtime: 2/11/04 11:43 $
// $Revision: 3 $
// $Workfile: MIL_BouchonMines.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_BouchonMines::Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army )
{
    return *new MIL_BouchonMines( type, nID, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_BouchonMines::GetMinesActivityTime
// Created: NLD 2007-02-08
// -----------------------------------------------------------------------------
inline
uint MIL_BouchonMines::GetMinesActivityTime() const
{
    return nMinesActivityTime_;
}
