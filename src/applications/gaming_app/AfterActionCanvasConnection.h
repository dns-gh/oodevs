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
             AfterActionCanvasConnection( const QPalette& palette, AfterActionCanvasItem* from, int index, const QPoint& attach );
    virtual ~AfterActionCanvasConnection();
    //@}

    //! @name Operations
    //@{
    void Move( const QPoint& to );
    void MoveBy( AfterActionCanvasItem* item, double x, double y );
    bool Close( AfterActionCanvasItem* item, int index, const QPoint& attach );
    void Disconnect( AfterActionCanvasItem* item = 0 );
    bool IsConnected( AfterActionCanvasItem* item, int index ) const;
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
    void Remove( AfterActionCanvasItem*& mine, AfterActionCanvasItem* item );
    //@}

private:
    //! @name Member data
    //@{
    const QPalette& palette_;
    AfterActionCanvasItem* from_; int fromIndex_;
    AfterActionCanvasItem* to_;   int toIndex_;
    //@}
};

#endif // __AfterActionCanvasConnection_h_
