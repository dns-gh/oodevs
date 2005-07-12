//*****************************************************************************
//
// $Created: FBD 02-11-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Scroller.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_Scroller.h $
//
//*****************************************************************************

#ifndef __MOS_Scroller_h_
#define __MOS_Scroller_h_

#include "MOS_Light_Pch.h"


#include <QButtonGroup.h>

//*****************************************************************************
// Created: FBD 02-11-29
//*****************************************************************************
class MOS_Scroller : public QButtonGroup 
{
public:
    explicit MOS_Scroller( QWidget* pParent, QGLWidget* pGLWidget );
    virtual ~MOS_Scroller();

private:
};

#ifdef MOS_USE_INLINE
#	include "MOS_Scroller.inl"
#endif


#endif // __MOS_Scroller_h_