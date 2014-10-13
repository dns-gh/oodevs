// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_h_
#define __Automat_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "DrawableUnitContainer.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class AutomatType;
}

namespace sword
{
    class AutomatCreation;
    class AutomatAttributes;
}

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat : public gui::EntityImplementation< kernel::Automat_ABC >
              , public kernel::Extension_ABC
              , public gui::Drawable_ABC
              , public kernel::Updatable_ABC< sword::AutomatAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const sword::AutomatCreation& message,
                      kernel::Controller& controller,
                      const kernel::AutomatType& type,
                      const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::AutomatType& GetType() const;
    virtual void DoUpdate( const sword::AutomatAttributes& message );
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AutomatType& type_;
    DrawableUnitContainer drawable_;
    //@}
};

#endif // __Automat_h_
