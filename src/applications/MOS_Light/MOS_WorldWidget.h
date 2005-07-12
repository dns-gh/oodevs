//*****************************************************************************
//
// $Created: NLD 2002-11-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_WorldWidget.h $
// $Author: Agn $
// $Modtime: 20/04/04 16:55 $
// $Revision: 5 $
// $Workfile: MOS_WorldWidget.h $
//
//*****************************************************************************

#ifndef __MOS_WorldWidget_h_
#define __MOS_WorldWidget_h_


#include "MOS_Types.h"
#include "MOS_GLWidget.h"

#include <qframe.h>

class QSlider;
class MOS_LimaEditor;
class MOS_LimaOrder;
class MOS_LimaSelector;
class MOS_AgentListView;
class MOS_GLMouse_ABC;
class MOS_Scroller;
class MOS_ZoomSlider;

//=============================================================================
// Created: NLD 2002-11-28
//=============================================================================
class MOS_WorldWidget : public QFrame
{
    MT_COPYNOTALLOWED( MOS_WorldWidget );

public:
    MOS_WorldWidget( QWidget* pParent );
    virtual ~MOS_WorldWidget();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    void Initialize();
    void Terminate ();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_GLWidget& GetGLWidget() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void RegisterLink( MOS_GLMouse_ABC* pLink );
    void UnregisterLink( MOS_GLMouse_ABC* pLink );
    //@}
protected:
    virtual void wheelEvent( QWheelEvent* pEvent );

private:
    MOS_GLWidget* pGLWidget_;
    MOS_ZoomSlider*      pZoomSlider_;
    MOS_Scroller* pScroller_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_WorldWidget.inl"
#endif

#endif // __MOS_WorldWidget_h_
