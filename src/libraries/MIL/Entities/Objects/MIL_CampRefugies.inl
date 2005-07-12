//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_CampRefugies.inl $
// $Author: Nld $
// $Modtime: 10/03/05 10:35 $
// $Revision: 2 $
// $Workfile: MIL_CampRefugies.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_CampRefugies::Create()
{
    return *new MIL_CampRefugies();
}

// -----------------------------------------------------------------------------
// Name: MIL_CampRefugies::GetTC2
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG& MIL_CampRefugies::GetTC2() const
{
    assert( pTC2_ );
    return *pTC2_;
}
