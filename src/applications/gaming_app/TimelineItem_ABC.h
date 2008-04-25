// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineItem_ABC_h_
#define __TimelineItem_ABC_h_

#include <qcanvas.h>

// =============================================================================
/** @class  TimelineItem_ABC
    @brief  TimelineItem_ABC
*/
// Created: SBO 2007-07-06
// =============================================================================
class TimelineItem_ABC : public QCanvasRectangle
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TimelineItem_ABC( QCanvas* canvas );
             TimelineItem_ABC( QCanvas* canvas, const QRect& area );
    virtual ~TimelineItem_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Update() = 0;
    virtual void Shift( long shift );
    virtual void DisplayToolTip( QWidget* parent ) const; // $$$$ SBO 2007-07-19: make a displayer maybe...
    virtual void DisplayContextMenu( QWidget* parent, const QPoint& pos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineItem_ABC( const TimelineItem_ABC& );            //!< Copy constructor
    TimelineItem_ABC& operator=( const TimelineItem_ABC& ); //!< Assignment operator
    //@}
};

#endif // __TimelineItem_ABC_h_
