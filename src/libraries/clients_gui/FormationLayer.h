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

#include "EntityLayer.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/Formation_ABC.h"

namespace kernel
{
    class Formation_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;

// =============================================================================
/** @class  FormationLayer
    @brief  Formation layer
*/
// Created: AGE 2007-05-31
// =============================================================================
class FormationLayer : public EntityLayer< kernel::Formation_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             FormationLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, ColorStrategy_ABC& strategy, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~FormationLayer();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationLayer( const FormationLayer& );            //!< Copy constructor
    FormationLayer& operator=( const FormationLayer& ); //!< Assignment operator
    //@}

protected:
    //! @name Operations
    //@{
    virtual void ActivateEntity( const kernel::Entity_ABC& );
    virtual void Draw( const kernel::Entity_ABC& entity, kernel::Viewport_ABC& viewport );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    //@}
};

}

#endif // __FormationLayer_h_
