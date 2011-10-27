// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExclusiveEventStrategy_h_
#define __ExclusiveEventStrategy_h_

#include <graphics/EventStrategy_ABC.h>

namespace gui
{

// =============================================================================
/** @class  ExclusiveEventStrategy
    @brief  Exclusive Event Strategy
*/
// Created: AGE 2006-09-04
// =============================================================================
class ExclusiveEventStrategy : public EventStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ExclusiveEventStrategy( EventStrategy_ABC& forward );
    virtual ~ExclusiveEventStrategy();
    //@}

    //! @name Operations
    //@{
    void TakeExclusiveFocus( MapLayer_ABC& layer );
    void ReleaseExclusiveFocus();
    //@}

    //! @name Operations
    //@{
    virtual void Register( MapLayer_ABC& layer );
    virtual void Remove  ( MapLayer_ABC& layer );

    virtual void HandleKeyPress        ( QKeyEvent* key );
    virtual void HandleKeyRelease      ( QKeyEvent* key );
    virtual void HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseWheel      ( QWheelEvent* mouse, const geometry::Point2f& point );
    virtual void HandleDropEvent       ( QDropEvent*  event, const geometry::Point2f& point );
    virtual void HandleEnterDragEvent  ( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual void HandleMoveDragEvent   ( QDragMoveEvent*  event, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ExclusiveEventStrategy( const ExclusiveEventStrategy& );            //!< Copy constructor
    ExclusiveEventStrategy& operator=( const ExclusiveEventStrategy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    EventStrategy_ABC& forward_;
    MapLayer_ABC* layer_;
    //@}
};

}

#endif // __ExclusiveEventStrategy_h_
