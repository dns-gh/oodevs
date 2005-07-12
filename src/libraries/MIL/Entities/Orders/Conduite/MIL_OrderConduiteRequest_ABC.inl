//*****************************************************************************
//
// $Created: NLD 2003-01-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.inl $
// $Author: Nld $
// $Modtime: 3/09/04 18:17 $
// $Revision: 1 $
// $Workfile: MIL_OrderConduiteRequest_ABC.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC::GetName
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
inline
const char* MIL_OrderConduiteRequest_ABC::GetName() const
{
    return "";
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderConduiteRequest_ABC::GetType
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
inline
const MIL_OrderConduiteType& MIL_OrderConduiteRequest_ABC::GetType() const
{
    return type_;
}
