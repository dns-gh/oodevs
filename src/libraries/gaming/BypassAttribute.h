// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BypassAttribute_h_
#define __BypassAttribute_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  BypassAttribute
    @brief  BypassAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class BypassAttribute : public kernel::BypassAttribute_ABC
                      , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BypassAttribute( kernel::Controller& controller, bool hasSinglePointPos = false );
    virtual ~BypassAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    int GetBypassConstructionPercentage() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BypassAttribute( const BypassAttribute& );            //!< Copy constructor
    BypassAttribute& operator=( const BypassAttribute& ); //!< Assignment operator
    //@}

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
    int nBypassConstructionPercentage_;
    bool hasSinglePointPos_;
    //@}
};

#endif // __BypassAttribute_h_
