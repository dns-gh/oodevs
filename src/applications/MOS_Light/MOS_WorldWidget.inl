//*****************************************************************************
//
// $Created: NLD 2002-11-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_WorldWidget.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 6 $
// $Workfile: MOS_WorldWidget.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget::GetGLWidget
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
MOS_GLWidget& MOS_WorldWidget::GetGLWidget() const
{
    assert( pGLWidget_ ); 
    return *pGLWidget_;
}


//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget::RegisterLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
inline
void MOS_WorldWidget::RegisterLink( MOS_GLMouse_ABC* pLink )
{
    pGLWidget_->RegisterLink( pLink );
}


//-----------------------------------------------------------------------------
// Name: MOS_WorldWidget::UnregisterLink
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
inline
void MOS_WorldWidget::UnregisterLink( MOS_GLMouse_ABC* pLink )
{
    pGLWidget_->UnregisterLink( pLink );
}
