// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/GL3DWidget.h $
// $Author: Age $
// $Modtime: 17/05/05 14:00 $
// $Revision: 3 $
// $Workfile: GL3DWidget.h $
//
// *****************************************************************************

#ifndef __GL3DWidget_h_
#define __GL3DWidget_h_

#include "graphics/MapWidget3D.h"
#include "Types.h"
#include "MapEventFilter_ABC.h"

class MT_GLDrawer;
class QTimer;

// =============================================================================
/** @class  GL3DWidget
    @brief  GL3DWidget
*/
// Created: AGE 2005-05-09
// =============================================================================
class GL3DWidget : public MapWidget3D
{
    Q_OBJECT;
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
             GL3DWidget( QWidget* pParent );
    virtual ~GL3DWidget();
    //@}

    //! @name Operations
    //@{
    const T_MapEventFilterVector& GetMapEvents() const;

    virtual void hideEvent( QHideEvent* );
    virtual void showEvent( QShowEvent* );
    virtual bool event( QEvent* pEvent );
    virtual void mouseMoveEvent( QMouseEvent* pMouseEvent );
    virtual void paintGL();
    virtual void initializeGL();
    virtual void keyPressEvent( QKeyEvent* );

    MT_Vector2D ScreenToGL ( const MT_Vector2D& v );
    MT_Vector2D ScreenToGL ( const QMouseEvent& event );
    MT_Vector2D ScreenToGL ( const QPoint& vScreenPos );

    void PushMapEventFilter( MapEventFilter_ABC& filter );
    void PopMapEventFilter( MapEventFilter_ABC& filter );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnCenterOnPoint( const MT_Vector2D& vPoint );
    //@}

signals:
    //! @name Signals
    //@{
    void MouseMove( QMouseEvent* pMouseEvent, const MT_Vector2D& vGLPos );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GL3DWidget( const GL3DWidget& );            //!< Copy constructor
    GL3DWidget& operator=( const GL3DWidget& ); //!< Assignement operator
    //@}

private:
    enum E_RulerState
    {
        eNone,
        eStarting,
        eExistant
    };

private:
    MT_Rect      viewport_;
    MT_Float     rMetersPerPixel_;
    E_RulerState nRulerState_;
    MT_Vector2D  vRulerStartPos_;
    MT_Vector2D  vRulerEndPos_;

    T_MapEventFilterVector eventFilterStack_;

private:
    //! @name Member data
    //@{
    MT_GLDrawer* pGLDrawer_;
    float        rZFactor_;
    QTimer*      pTimer_;
    //@}
};

#endif // __GL3DWidget_h_
