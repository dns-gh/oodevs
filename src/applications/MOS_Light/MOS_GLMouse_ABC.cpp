//*****************************************************************************
//
// $Created: FBD 03-01-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GLMouse_ABC.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_GLMouse_ABC.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_GLMouse_ABC.h"

#ifndef MOS_USE_INLINE
#	include "MOS_GLMouse_ABC.inl"
#endif

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"

//-----------------------------------------------------------------------------
// Name: MOS_GLMouse_ABC constructor
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_GLMouse_ABC::MOS_GLMouse_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_GLMouse_ABC destructor
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_GLMouse_ABC::~MOS_GLMouse_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_GLMouse_ABC::Initialize
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLMouse_ABC::Initialize()
{
    MOS_App::GetApp().GetMainWindow().GetWorldWidget().RegisterLink( this );
}
//-----------------------------------------------------------------------------
// Name: MOS_GLMouse_ABC::Terminate
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_GLMouse_ABC::Terminate()
{
    MOS_App::GetApp().GetMainWindow().GetWorldWidget().UnregisterLink( this );
}
