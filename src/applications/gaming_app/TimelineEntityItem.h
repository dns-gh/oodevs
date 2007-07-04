// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineEntityItem_h_
#define __TimelineEntityItem_h_

#include <qcanvas.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class Action_ABC;
class TimelineActionItem;

// =============================================================================
/** @class  TimelineEntityItem
    @brief  TimelineEntityItem
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineEntityItem : public QCanvasRectangle
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineEntityItem( QCanvas* canvas, QCanvasView* view, const kernel::Entity_ABC& entity );
    virtual ~TimelineEntityItem();
    //@}

    //! @name Operations
    //@{
    void NotifyCreated( const Action_ABC& action );
    void NotifyUpdated( const Action_ABC& action );
    void NotifyDeleted( const Action_ABC& action );
    //@}

    //! @name Display
    //@{
    void SetYOffset( unsigned int lineIndex );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineEntityItem( const TimelineEntityItem& );            //!< Copy constructor
    TimelineEntityItem& operator=( const TimelineEntityItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void draw( QPainter& painter );
    virtual QPointArray chunks() const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const Action_ABC*, TimelineActionItem* > T_Items;
    typedef T_Items::const_iterator                          CIT_Items;
    //@}

private:
    //! @name Member data
    //@{
    QCanvasView& view_;
    const kernel::Entity_ABC& entity_;
    T_Items items_;
    //@}
};

#endif // __TimelineEntityItem_h_
