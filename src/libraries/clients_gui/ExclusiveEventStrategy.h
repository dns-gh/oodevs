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
    class Layer_ABC;

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
    void TakeExclusiveFocus( const std::shared_ptr< Layer_ABC >& layer );
    void ReleaseExclusiveFocus();
    //@}

    //! @name Operations
    //@{
    virtual void HandleKeyPress        ( QKeyEvent* key );
    virtual void HandleKeyRelease      ( QKeyEvent* key );
    virtual void HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseRelease    ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseWheel      ( QWheelEvent* mouse, const geometry::Point2f& point );
    virtual void HandleDropEvent       ( QDropEvent*  event, const geometry::Point2f& point );
    virtual void HandleEnterDragEvent  ( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual void HandleMoveDragEvent   ( QDragMoveEvent*  event, const geometry::Point2f& point );
    virtual void HandleLeaveDragEvent  ( QDragLeaveEvent* event );
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
    std::shared_ptr< Layer_ABC > layer_;
    //@}
};

}

#endif // __ExclusiveEventStrategy_h_
