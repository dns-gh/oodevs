// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_gui/EntityImplementation.h"
#include "DrawableUnitContainer.h"
#include "clients_kernel/Formation_ABC.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class FormationCreation;
    class FormationUpdate;
}

namespace kernel
{
    class StaticModel;
}

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2006-10-19
// =============================================================================
class Formation : public gui::EntityImplementation< kernel::Formation_ABC >
                , public kernel::Updatable_ABC< sword::FormationUpdate >
                , public kernel::Extension_ABC
                , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const sword::FormationCreation& message,
                        kernel::Controller& controller,
                        const T_CanBeRenamedFunctor& canBeRenamedFunctor );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual E_NatureLevel GetLevel() const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeSymbol() const;
    void DoUpdate( const sword::FormationUpdate& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller&   controller_;
    E_NatureLevel         level_;
    DrawableUnitContainer drawable_;
    //@}
};

#endif // __Formation_h_
