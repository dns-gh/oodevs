// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationLayer_h_
#define __FormationLayer_h_

#include "Layer_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Formation_ABC;
    class GlTools_ABC;
    class Controllers;
}

namespace gui
{
    class ColorStrategy_ABC;

// =============================================================================
/** @class  FormationLayer
    @brief  FormationLayer
*/
// Created: AGE 2007-05-31
// =============================================================================
class FormationLayer : public Layer_ABC
                     , public kernel::Observer_ABC
                     , public kernel::SelectionObserver< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy );
    virtual ~FormationLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationLayer( const FormationLayer& );            //!< Copy constructor
    FormationLayer& operator=( const FormationLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Formation_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    kernel::SafePointer< kernel::Formation_ABC > selected_;
    //@}
};

}

#endif // __FormationLayer_h_
