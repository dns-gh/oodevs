// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionAttribute_h_
#define __ConstructionAttribute_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class DotationType;
}

// =============================================================================
/** @class  ConstructionAttribute
    @brief  ConstructionAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ConstructionAttribute : public kernel::ConstructionAttribute_ABC
                            , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ConstructionAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType >& resolver, bool singlePointPos = false );
    virtual ~ConstructionAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    int GetConstructionPercentage() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConstructionAttribute( const ConstructionAttribute& );            //!< Copy constructor
    ConstructionAttribute& operator=( const ConstructionAttribute& ); //!< Assignment operator
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
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    kernel::DotationType*                              construction_;
    kernel::OptionalValue< int >                       nConstructionPercentage_;
    kernel::OptionalValue< unsigned int >              nDotationConstruction_;
    bool hasSinglePointPos_;
    //@}
};

#endif // __ConstructionAttribute_h_
