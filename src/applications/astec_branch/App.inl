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
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/App.inl $
// $Author: Age $
// $Modtime: 14/03/05 14:01 $
// $Revision: 9 $
// $Workfile: App.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: App::GetApp
// Created: DFT 02-03-13
//-----------------------------------------------------------------------------
inline
App& App::GetApp()
{
    assert( pInstance_ != 0 );
	return *pInstance_;
}


//-----------------------------------------------------------------------------
// Name: App::GetMOSServer
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
inline
Network& App::GetNetwork() const
{
    assert( pMOSServer_ );
    return *pMOSServer_;
}

//-----------------------------------------------------------------------------
// Name: App::GetWorld
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
inline
World& App::GetWorld() const
{
    assert( pWorld_ != 0 );
    return *pWorld_;
}

// -----------------------------------------------------------------------------
// Name: App::GetMainWindow
/** @return 
*/
// Created: APE 2004-04-13
// -----------------------------------------------------------------------------
inline
MainWindow& App::GetMainWindow() const
{
    assert( pMainWindow_ != 0 );
    return *pMainWindow_;
}
