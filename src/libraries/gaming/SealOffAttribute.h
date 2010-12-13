// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SealOffAttribute_h_
#define __SealOffAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "clients_kernel/Drawable_ABC.h"

namespace kernel
{
    class Controller;
    class Displayer_ABC;
}

// =============================================================================
/** @class  SealOffAttribute
    @brief  SealOffAttribute
*/
// Created: MGD 2010-08-24
// =============================================================================
class SealOffAttribute : public kernel::SealOffAttribute_ABC
                       , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SealOffAttribute( kernel::Controller& controller );
    virtual ~SealOffAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SealOffAttribute( const SealOffAttribute& );            //!< Copy constructor
    SealOffAttribute& operator=( const SealOffAttribute& ); //!< Assignment operator
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
    float rSealOffLevel_;
    //@}
};

#endif // __SealOffAttribute_h_
