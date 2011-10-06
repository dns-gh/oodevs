// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CircularEventStrategy_h_
#define __CircularEventStrategy_h_

#include <graphics/EventStrategy_ABC.h>

namespace gui
{

// =============================================================================
/** @class  CircularEventStrategy
    @brief  CircularEventStrategy
*/
// Created: AGE 2006-08-21
// =============================================================================
class CircularEventStrategy : public EventStrategy_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CircularEventStrategy();
    virtual ~CircularEventStrategy();
    //@}

    //! @name Settings
    //@{
    void SetReverse( bool );
    void SetExclusive( bool );
    //@}

    //! @name Operations
    //@{
    void SetDefault( MapLayer_ABC& layer );
    void Register  ( MapLayer_ABC& layer );
    void Remove    ( MapLayer_ABC& layer );

    virtual void HandleKeyPress        ( QKeyEvent* key );
    virtual void HandleKeyRelease      ( QKeyEvent* key );
    virtual void HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseWheel      ( QWheelEvent* mouse,     const geometry::Point2f& point );
    virtual void HandleDropEvent       ( QDropEvent*  event,     const geometry::Point2f& point );
    virtual void HandleEnterDragEvent  ( QDragEnterEvent* event, const geometry::Point2f& point );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CircularEventStrategy( const CircularEventStrategy& );            //!< Copy constructor
    CircularEventStrategy& operator=( const CircularEventStrategy& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename It, typename Functor >
    bool Loop( It& use, It first, It begin, It end, Functor functor );

    template< typename Functor >
    bool Apply( Functor functor );
    //@}

private:
    //! @name Member data
    //@{
    T_MapLayers layers_;
    MapLayer_ABC* default_;
    bool reverse_;
    bool exclusive_;
    CIT_MapLayers last_;
    CRIT_MapLayers rlast_;
    //@}
};

}

#endif // __CircularEventStrategy_h_
