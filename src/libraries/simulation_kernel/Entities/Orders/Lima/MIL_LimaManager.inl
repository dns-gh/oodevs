//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Lima/MIL_LimaManager.inl $
// $Author: Nld $
// $Modtime: 21/10/04 15:59 $
// $Revision: 2 $
// $Workfile: MIL_LimaManager.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MIL_LimaManager::FindLima
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
const MIL_Lima* MIL_LimaManager::FindLima( uint nID ) const
{
    CIT_LimaMap itLima = limaMap_.find( nID );
    if( itLima == limaMap_.end() )
        return 0;
    return itLima->second;
}


//-----------------------------------------------------------------------------
// Name: MIL_LimaManager::DestroyLima
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
bool MIL_LimaManager::DestroyLima( MIL_Lima& lima )
{
    return DestroyLima( lima.GetID() );
}
