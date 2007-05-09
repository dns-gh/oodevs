// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Role_ABC.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 14:45 $
// $Revision: 2 $
// $Workfile: MT_Role_ABC.cpp $
//
// *****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_Role_ABC.h"

//-----------------------------------------------------------------------------
// Name: MT_Role_ABC::MT_Role_ABC
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
MT_Role_ABC::MT_Role_ABC( MT_RoleContainer& roleContainer )
    : pRoleContainer_( &roleContainer )
{
}

// -----------------------------------------------------------------------------
// Name: MT_Role_ABC constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MT_Role_ABC::MT_Role_ABC()
    : pRoleContainer_( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: MT_Role_ABC::~MT_Role_ABC
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
MT_Role_ABC::~MT_Role_ABC()
{
}

