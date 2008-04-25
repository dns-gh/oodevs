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
class TimelineView;

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
             TimelineEntityItem( TimelineView& view, kernel::Controllers& controllers, const kernel::Entity_ABC& entity );
    virtual ~TimelineEntityItem();
    //@}

    //! @name Operations
    //@{
    virtual void Update();

    void MoveAfter( const TimelineItem_ABC* previous );
    void AddAction( const Action_ABC& action );
    void RemoveAction( const Action_ABC& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineEntityItem( const TimelineEntityItem& );            //!< Copy constructor
    TimelineEntityItem& operator=( const TimelineEntityItem& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Action_ABC*, TimelineItem_ABC* > T_Items;
    //@}

private:
    //! @name Member data
    //@{
    TimelineView& view_;
    kernel::Controllers& controllers_;
    const kernel::Entity_ABC& entity_;
    T_Items childItems_;
    //@}
};

#endif // __TimelineEntityItem_h_
