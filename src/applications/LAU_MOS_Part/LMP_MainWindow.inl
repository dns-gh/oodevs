//*****************************************************************************
//
// $Created : AML 03-04-02 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/LMP_MainWindow.inl $
//   $Author: Aml $
//  $Modtime: 17/04/03 15:18 $
// $Revision: 4 $
// $Workfile: LMP_MainWindow.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: LMP_MainWindow::GetMainWindow
// Created: AML 03-04-04
//-----------------------------------------------------------------------------
INLINE
LMP_MainWindow* LMP_MainWindow::GetMainWindow( void )
{
    return static_pTheOnlyMainWindow;
}
