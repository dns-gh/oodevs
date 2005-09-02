//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectManager.inl $
// $Author: Ape $
// $Modtime: 5/05/04 15:56 $
// $Revision: 2 $
// $Workfile: MOS_DynaObjectManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::FindDynaObject
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_DynaObject_ABC* MOS_DynaObjectManager::FindDynaObject( uint nID ) const
{
    CIT_DynaObjectMap itObject = dynaObjectMap_.find( nID );
    if( itObject == dynaObjectMap_.end() )
        return 0;
    return itObject->second;
}


// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::GetDynaObjects
/** @return 
*/
// Created: APE 2004-05-05
// -----------------------------------------------------------------------------
MOS_INLINE
const MOS_DynaObjectManager::T_DynaObjectMap& MOS_DynaObjectManager::GetDynaObjects() const
{
    return dynaObjectMap_;
}
