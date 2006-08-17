//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_CampPrisonniers.inl $
// $Author: Nld $
// $Modtime: 7/03/05 15:32 $
// $Revision: 2 $
// $Workfile: MIL_CampPrisonniers.inl $
//
//*****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::Create
// Created: NLD 2004-09-16
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC& MIL_CampPrisonniers::Create()
{
    return *new MIL_CampPrisonniers();
}

// -----------------------------------------------------------------------------
// Name: MIL_CampPrisonniers::GetTC2
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
inline
MIL_AutomateLOG& MIL_CampPrisonniers::GetTC2() const
{
    assert( pTC2_ );
    return *pTC2_;
}
