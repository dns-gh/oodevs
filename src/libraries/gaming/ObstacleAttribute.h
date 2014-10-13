// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObstacleAttribute_h_
#define __ObstacleAttribute_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  ObstacleAttribute
    @brief  ObstacleAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ObstacleAttribute : public kernel::ObstacleAttribute_ABC
                        , public gui::Drawable_ABC
                        , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObstacleAttribute( kernel::Controller& controller, bool singlePointPos = false );
    virtual ~ObstacleAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;

    virtual bool IsObstacleActivated() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::OptionalValue< bool > obstacleActivated_;
    kernel::OptionalValue< unsigned int > activationTime_;
    kernel::OptionalValue< unsigned int > activityTime_;
    bool hasSinglePointPos_;
    //@}
};

#endif // __ObstacleAttribute_h_
