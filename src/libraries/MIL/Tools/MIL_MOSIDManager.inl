//*****************************************************************************
//
// $Created: NLD 2003-02-28 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Tools/MIL_MOSIDManager.inl $
// $Author: Nld $
// $Modtime: 13/04/05 14:51 $
// $Revision: 4 $
// $Workfile: MIL_MOSIDManager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::SetClassID
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
inline
void MIL_MOSIDManager::SetClassID( uint nMOSClassID )
{
    nMOSClassID_ = nMOSClassID;
}


//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::IsMosIDValid
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
inline
bool MIL_MOSIDManager::IsMosIDValid( uint nID )
{
    if( nID >> 22 != nMOSClassID_ )
        return false;

    if( (nID & 0x3FFFFF) >= nSimStartID_ )
        return false;

    return true;
}

//-----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::IsSimIDValid
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
inline
bool MIL_MOSIDManager::IsSimIDValid( uint nID )
{
    if( nID >> 22 != nMOSClassID_ )
        return false;

    if( (nID & 0x3FFFFF) < nSimStartID_ )
        return false;

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_MOSIDManager::GetClassID
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
inline
uint MIL_MOSIDManager::GetClassID() const
{
    return nMOSClassID_;
}
