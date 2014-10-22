// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodAttribute_h_
#define __FloodAttribute_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class AltitudeModified;
    class Controller;
    class Positions;
}

class FloodProxy;

// =============================================================================
/** @class  FloodAttribute
    @brief  FloodAttribute
*/
// Created: JSR 2010-12-15
// =============================================================================
class FloodAttribute : public kernel::FloodAttribute_ABC
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< kernel::AltitudeModified >
                     , public gui::Drawable_ABC
                     , public boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodAttribute( kernel::Controller& controller, FloodProxy& proxy, const kernel::Positions& positions );
    virtual ~FloodAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& view ) const;
    virtual void NotifyUpdated( const kernel::AltitudeModified& attribute );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message, bool isReal );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Positions& positions_;
    FloodProxy& proxy_;
    unsigned int floodId_;
    bool isReal_;
    int depth_;
    int refDist_;
    //@}
};

#endif // __FloodAttribute_h_
