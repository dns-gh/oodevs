// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ObjectManager.inl $
// $Author: Nld $
// $Modtime: 26/10/04 16:03 $
// $Revision: 2 $
// $Workfile: MIL_ObjectManager.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: MIL_ObjectManager::FindObject
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
inline
MIL_RealObject_ABC* MIL_ObjectManager::FindRealObject( uint nID ) const
{
    CIT_RealObjectMap it = realObjects_.find( nID );
    if( it == realObjects_.end() )
        return 0;
    return it->second;
}
