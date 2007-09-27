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

class AfterActionItem_ABC;
class AfterActionFunction;
class AfterActionCanvasItem;
class AfterActionCanvasConnection;

// =============================================================================
/** @class  AfterActionCanvas
    @brief  After action canvas
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionCanvas : public QCanvasView
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit AfterActionCanvas( QWidget* parent );
    virtual ~AfterActionCanvas();
    //@}

    //! @name Operations
    //@{
    void Edit( const AfterActionFunction* function );
    AfterActionCanvasItem* Resolve( AfterActionItem_ABC* item ) const;
    //@}

private slots:
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
    virtual void resizeEvent( QResizeEvent* event );
    void ClearSelection();
    void Clear();

    void Connect( QMouseEvent* event );
    void Move( QMouseEvent* event );
    void DeleteSelected();

    void Add( AfterActionItem_ABC& i, const QPoint& position = QPoint() );
    //@}

    //! @name Types
    //@{
    typedef std::vector< AfterActionCanvasItem* >  T_Items;
    typedef T_Items::iterator                     IT_Items;
    //@}

private:
    //! @name Member data
    //@{
    AfterActionFunction* function_;
    T_Items items_;
    AfterActionCanvasItem*       selected_;
    AfterActionCanvasConnection* selectedConnection_;
    AfterActionCanvasConnection* currentConnection_;
    QPoint grabPoint_;
    bool connect_;
    //@}
};

#endif // __AfterActionCanvas_h_
