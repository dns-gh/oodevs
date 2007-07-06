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

#include "TimelineItem_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class Action_ABC;

// =============================================================================
/** @class  TimelineEntityItem
    @brief  TimelineEntityItem
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineEntityItem : public TimelineItem_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineEntityItem( QCanvasView& view, const TimelineItem_ABC* after, kernel::Controllers& controllers, const kernel::Entity_ABC& entity );
    virtual ~TimelineEntityItem();
    //@}

    //! @name Operations
    //@{
    void AddAction( const Action_ABC& action );
    void RemoveAction( const Action_ABC& action );
    void Update();
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
    //@}

    //! @name Types
    //@{
    typedef std::map< const Action_ABC*, TimelineItem_ABC* > T_Items;
    typedef T_Items::const_iterator                        CIT_Items;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QCanvasView& view_;
    const TimelineItem_ABC* previous_;
    const kernel::Entity_ABC& entity_;
    T_Items childItems_;
    //@}
};

#endif // __TimelineEntityItem_h_
