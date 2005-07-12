//*****************************************************************************
//
// $Created: FBD 02-11-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GLWidget.inl $
// $Author: Age $
// $Modtime: 7/02/05 17:07 $
// $Revision: 8 $
// $Workfile: MOS_GLWidget.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::SetCenter
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
inline
void MOS_GLWidget::SetCenter( const MT_Vector2D& vCenter )
{
    vCenter_ = vCenter;
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::SetCurrentGL
// Created: FBD 03-01-30
//-----------------------------------------------------------------------------
inline
void MOS_GLWidget::SetCurrentGL()
{
    makeCurrent();
}


//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::GetNbLink
// Created: AGN 03-09-04
//-----------------------------------------------------------------------------
inline
int MOS_GLWidget::GetNbLink() const
{
    return linkVector_.size();
}

//-----------------------------------------------------------------------------
// Name: MOS_GLWidget::GetNbLink
// Created: APE
//-----------------------------------------------------------------------------
inline
MT_Vector2D MOS_GLWidget::GetCenter() const
{
    return vCenter_;
}