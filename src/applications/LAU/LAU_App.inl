//*****************************************************************************
//
// Name     : LAU_App.inl
//
// $Created : AML 03-03-27 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_App.inl $
// $Author: Nld $
// $Modtime: 5/05/04 10:42 $
// $Revision: 5 $
// $Workfile: LAU_App.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: LAU_App::GetWorkspace
// Created: AML 03-06-16
//-----------------------------------------------------------------------------
INLINE
const LAU_Workspace& LAU_App::GetWorkspace() const
{
    assert( pWorkSpace_ );
    return *pWorkSpace_;
}



//-----------------------------------------------------------------------------
// Name: LAU_App::GetWorkspace
// Created: AML 03-06-16
//-----------------------------------------------------------------------------
INLINE
LAU_Workspace& LAU_App::GetWorkspace()
{
    assert( pWorkSpace_ );
    return *pWorkSpace_;
}



// -----------------------------------------------------------------------------
// Name   : LAU_App::IsKillStuckedSim
// Created: Alma 2004-01-30
// -----------------------------------------------------------------------------
INLINE
bool32 LAU_App::IsKillStuckedSim( void ) const
{
    return bKillStuckedSim_;
}
