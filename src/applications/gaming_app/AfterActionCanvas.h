// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionCanvas_h_
#define __AfterActionCanvas_h_

#include <qcanvas.h>

class AfterActionItem;
class AfterActionCanvasItem;
class AfterActionCanvasConnection;

// =============================================================================
/** @class  AfterActionCanvas
    @brief  AfterActionCanvas
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionCanvas : public QCanvasView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionCanvas( QWidget* parent );
    virtual ~AfterActionCanvas();
    //@}


public slots:
    //! @name Slots
    //@{
    void Connect();
    void Select();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionCanvas( const AfterActionCanvas& );            //!< Copy constructor
    AfterActionCanvas& operator=( const AfterActionCanvas& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void dragEnterEvent( QDragEnterEvent* event );
    virtual void dropEvent     ( QDropEvent* event );
    virtual void contentsMousePressEvent( QMouseEvent* event );
    virtual void contentsMouseReleaseEvent( QMouseEvent* event );
    virtual void contentsMouseMoveEvent( QMouseEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );
    void ClearSelection();

    void Connect( QMouseEvent* event );
    void Move( QMouseEvent* event );
    void DeleteSelected();
    //@}

    //! @name Types
    //@{
    typedef std::vector< AfterActionCanvasItem* >  T_Items;
    typedef T_Items::iterator                     IT_Items;
    //@}

private:
    //! @name Member data
    //@{
    T_Items items_;
    AfterActionCanvasItem* selected_;
    QPoint grabPoint_;
    bool connect_;
    unsigned currentId_;
    AfterActionCanvasConnection* currentConnection_;
    //@}
};

#endif // __AfterActionCanvas_h_
