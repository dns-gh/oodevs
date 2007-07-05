// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineActionItem_h_
#define __TimelineActionItem_h_

#include <qcanvas.h>

class TimelineEntityItem;
class Action_ABC;

// =============================================================================
/** @class  TimelineActionItem
    @brief  TimelineActionItem
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineActionItem : public QCanvasRectangle
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineActionItem( TimelineEntityItem& parent, const Action_ABC& action );
    virtual ~TimelineActionItem();
    //@}

    //! @name Operations
    //@{
    virtual void moveBy( double dx, double dy );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineActionItem( const TimelineActionItem& );            //!< Copy constructor
    TimelineActionItem& operator=( const TimelineActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void draw( QPainter& painter );
    //@}

private:
    //! @name Member data
    //@{
    TimelineEntityItem& parentItem_;
    const Action_ABC& action_;

    QPalette palette_;
    //@}
};

#endif // __TimelineActionItem_h_
