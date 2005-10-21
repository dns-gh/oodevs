// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-02-27 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GLWidget.h $
// $Author: Age $
// $Modtime: 19/05/05 15:29 $
// $Revision: 14 $
// $Workfile: MOS_GLWidget.h $
//
// *****************************************************************************

#ifndef __MOS_GLWidget_h_
#define __MOS_GLWidget_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_MapEventFilter_ABC.h"

class MOS_Agent;
class MOS_ActionContext;
class MT_GLDrawer;
class QPopupMenu;


// =============================================================================
// Created: APE 2004-02-27
// =============================================================================
class MOS_GLWidget : public QGLWidget
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_GLWidget );
    friend class MOS_GLTool;

public:
    //! @name Constructor/Destructor
    //@{
    MOS_GLWidget( QWidget* pParent, QGLWidget* pShare = 0 );
    ~MOS_GLWidget();
    //@}

public:
    //! @name Operations
    //@{
    MT_Vector2D ScreenToGL ( const MT_Vector2D& v );
    MT_Vector2D ScreenToGL ( const QMouseEvent& event );
    MT_Vector2D ScreenToGL ( const QPoint& vScreenPos );
    //@}

    //! @name Event management
    //@{
    const T_MapEventFilterVector& GetMapEvents      () const;
          void                    PushMapEventFilter( MOS_MapEventFilter_ABC& filter );
          void                    PopMapEventFilter ( MOS_MapEventFilter_ABC& filter );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnCenterOnPoint( const MT_Vector2D& vPoint );
    //@}

protected:
    //! @name Event Handlers
    //@{
    virtual void hideEvent( QHideEvent* );
    virtual void showEvent( QShowEvent* );
    void initializeGL     ();
    void resizeGL         ( int w, int h );
    void paintGL          ();

    bool event            ( QEvent*      pEvent      );
    void mousePressEvent  ( QMouseEvent* pMouseEvent );
    void mouseMoveEvent   ( QMouseEvent* pMouseEvent );
    void mouseReleaseEvent( QMouseEvent* pMouseEvent );
    void wheelEvent       ( QWheelEvent* pWheelEvent );
    void keyPressEvent    ( QKeyEvent*   pKeyEvent   );

    void Update           ( unsigned int nWait = 0 );
    //@}

    //! @name Tools
    //@{
    void CalculateViewport( int w, int h );
    //@}

signals:
    //! @name Signals
    //@{
    void MouseMove( QMouseEvent* pMouseEvent, const MT_Vector2D& vGLPos );
    //@}

private:
    //! @name Enums
    //@{
    enum E_RulerState
    {
        eNone,
        eStarting,
        eExistant
    };
    //@}

private:
    QTimer*      pTimer_;
    bool         bUpdating_;

    MT_Rect      viewport_;
    MT_Vector2D  vCenter_;
    MT_Float     rZoom_;
    MT_Float     rMetersPerPixel_;
    MT_Vector2D  vGrabPos_;

    E_RulerState nRulerState_;
    MT_Vector2D  vRulerStartPos_;
    MT_Vector2D  vRulerEndPos_;
    
    MT_GLDrawer* pGLDrawer_;

    T_MapEventFilterVector eventFilterStack_;
};

#endif // __MOS_GLWidget_h_
