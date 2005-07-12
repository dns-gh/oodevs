//*****************************************************************************
//
// $Created: NLD 2003-01-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObjectManager.inl $
// $Author: Nld $
// $Modtime: 7/03/05 13:27 $
// $Revision: 4 $
// $Workfile: MOS_DynaObjectManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::FindDynaObject
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
inline
MOS_DynaObject* MOS_DynaObjectManager::FindDynaObject( uint nID ) const
{
    CIT_DynaObjectMap itObject = dynaObjectMap_.find( nID );
    if( itObject == dynaObjectMap_.end() )
        return 0;
    return itObject->second;
}

// -----------------------------------------------------------------------------
// Name: MOS_DynaObjectManager::GetObjects
// Created: NLD 2005-03-07
// -----------------------------------------------------------------------------
inline
const MOS_DynaObjectManager::T_DynaObjectMap& MOS_DynaObjectManager::GetObjects() const
{
    return dynaObjectMap_;
}
