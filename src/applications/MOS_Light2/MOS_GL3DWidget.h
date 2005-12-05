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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_GL3DWidget.h $
// $Author: Age $
// $Modtime: 17/05/05 14:00 $
// $Revision: 3 $
// $Workfile: MOS_GL3DWidget.h $
//
// *****************************************************************************

#ifndef __MOS_GL3DWidget_h_
#define __MOS_GL3DWidget_h_

#include "graphics/MapWidget3D.h"
#include "MOS_Types.h"
#include "MOS_MapEventFilter_ABC.h"

class MT_GLDrawer;
class QTimer;

// =============================================================================
/** @class  MOS_GL3DWidget
    @brief  MOS_GL3DWidget
*/
// Created: AGE 2005-05-09
// =============================================================================
class MOS_GL3DWidget : public MapWidget3D
{
    Q_OBJECT;
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
             MOS_GL3DWidget( QWidget* pParent );
    virtual ~MOS_GL3DWidget();
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

    void PushMapEventFilter( MOS_MapEventFilter_ABC& filter );
    void PopMapEventFilter( MOS_MapEventFilter_ABC& filter );
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
    MOS_GL3DWidget( const MOS_GL3DWidget& );            //!< Copy constructor
    MOS_GL3DWidget& operator=( const MOS_GL3DWidget& ); //!< Assignement operator
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

#endif // __MOS_GL3DWidget_h_
