//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Pion/MIL_PionMission_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: MIL_PionMission_ABC.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetDirDanger
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& MIL_OrderContext::GetDirDanger() const
{
    return dirDanger_;
}
    
// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetFuseau
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& MIL_OrderContext::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::SetFuseau
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
void MIL_OrderContext::SetFuseau( const MIL_Fuseau& fuseau )
{
    fuseau_ = fuseau;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetLimas
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
inline
const T_LimaVector& MIL_OrderContext::GetLimas() const
{
    return limas_;
}

