//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectManager.inl $
// $Author: Ape $
// $Modtime: 5/05/04 15:56 $
// $Revision: 2 $
// $Workfile: MOS_ObjectManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_ObjectManager::FindObject
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Object_ABC* MOS_ObjectManager::FindObject( uint nID ) const
{
    CIT_ObjectMap itObject = ObjectMap_.find( nID );
    if( itObject == ObjectMap_.end() )
        return 0;
    return itObject->second;
}


// -----------------------------------------------------------------------------
// Name: MOS_ObjectManager::GetObjects
/** @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_ObjectManager::T_ObjectMap& MOS_ObjectManager::GetObjects() const
{
    return ObjectMap_;
}
