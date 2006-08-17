//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Network/NetworkManager.inl $
// $Author: Sbo $
// $Modtime: 12/05/05 16:13 $
// $Revision: 1 $
// $Workfile: NetworkManager.inl $
//
//*****************************************************************************

namespace TEST
{

//-----------------------------------------------------------------------------
// Name: NetworkManager::GetDINEngine
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
inline
DIN::DIN_Engine& NetworkManager::GetDINEngine()
{
    return dinEngine_;
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::GetConnectionHandler
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
inline
ConnectionHandler& NetworkManager::GetConnectionHandler() const
{
    assert( pConnectionHandler_ );
    return *pConnectionHandler_;
}


//-----------------------------------------------------------------------------
// Name: NetworkManager::GetMessageMgr
// Created: SBO 2005-05-10
//-----------------------------------------------------------------------------
inline
MessageManager& NetworkManager::GetMessageMgr() const
{
    assert( pMessageMgr_ );
    return *pMessageMgr_;
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::GetLink
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
inline
DIN::DIN_Link& NetworkManager::GetLink() const
{
    assert( pLink_ );
    return *pLink_;
}

//-----------------------------------------------------------------------------
// Name: NetworkManager::IsConnected
// Created: SBO 2005-05-12
//-----------------------------------------------------------------------------
inline
bool NetworkManager::IsConnected() const
{
    return ( pLink_ != 0 );
}

} // end namespace TEST
