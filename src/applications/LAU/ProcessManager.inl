//*****************************************************************************
//
// $Created: AML 03-04-25 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/ProcessManager.inl $
// $Author: Aml $
// $Modtime: 25/04/03 15:58 $
// $Revision: 2 $
// $Workfile: ProcessManager.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: ProcessManager::HasProcess
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
bool32 ProcessManager::HasProcess( void ) const
{
    return !processList_.empty();
}
