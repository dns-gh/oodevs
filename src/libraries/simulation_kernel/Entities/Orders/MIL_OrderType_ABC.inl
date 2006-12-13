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

//-----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetDIAType
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
inline
const DIA_TypeDef& MIL_OrderType_ABC::GetDIAType() const
{
    assert( pDIAType_ );
    return *pDIAType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetID
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
inline
uint MIL_OrderType_ABC::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderType_ABC::GetName
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
inline
const std::string& MIL_OrderType_ABC::GetName() const
{
    return strName_;
}
