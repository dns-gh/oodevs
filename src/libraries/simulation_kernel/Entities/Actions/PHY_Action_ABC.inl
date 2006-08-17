// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/PHY_Action_ABC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_Action_ABC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::Suspend
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
inline
void PHY_Action_ABC::Suspend()
{
    bSuspended_ = true;    
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::Resume
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
inline
void PHY_Action_ABC::Resume()
{
    bSuspended_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_Action_ABC::Update
// Created: NLD 2004-09-13
// -----------------------------------------------------------------------------
inline
void PHY_Action_ABC::Update()
{
    if( bSuspended_ )
        ExecuteSuspended();
    else
        Execute();
}
