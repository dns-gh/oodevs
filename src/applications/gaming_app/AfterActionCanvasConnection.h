// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionCanvasConnection_h_
#define __AfterActionCanvasConnection_h_

#include <qcanvas.h>
class AfterActionCanvasItem;
class AfterActionItem_ABC;

// =============================================================================
/** @class  AfterActionCanvasConnection
    @brief  AfterActionCanvasConnection
*/
// Created: AGE 2007-09-18
// =============================================================================
class AfterActionCanvasConnection : public QCanvasLine
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionCanvasConnection( const QPalette& palette, AfterActionCanvasItem* to, int index, double x, double y);
             AfterActionCanvasConnection( const QPalette& palette, AfterActionCanvasItem* from, double x, double y );
    virtual ~AfterActionCanvasConnection();
    //@}

    //! @name Operations
    //@{
    void Move( const QPoint& to );
    void MoveBy( AfterActionCanvasItem* item, double x, double y );
    void Close( AfterActionCanvasItem* from, AfterActionCanvasItem* to, int i, double x1, double y1, double x2, double y2 );

    AfterActionCanvasItem* From() const;
    AfterActionCanvasItem* To() const;
    int                    ToIndex() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionCanvasConnection( const AfterActionCanvasConnection& );            //!< Copy constructor
    AfterActionCanvasConnection& operator=( const AfterActionCanvasConnection& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void drawShape( QPainter& p );
    //@}

private:
    //! @name Member data
    //@{
    const QPalette& palette_;
    AfterActionCanvasItem* from_;
    AfterActionCanvasItem* to_; int toIndex_;
    //@}
};

#endif // __AfterActionCanvasConnection_h_
