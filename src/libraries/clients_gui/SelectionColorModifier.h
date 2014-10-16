// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SelectionColorModifier_h_
#define __SelectionColorModifier_h_

#include "ColorModifier_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Profile_ABC;
    class UrbanObject_ABC;
}

namespace gui
{
    class GLView_ABC;
}

namespace gui
{
// =============================================================================
/** @class  SelectionColorModifier
    @brief  SelectionColorModifier
*/
// Created: AGE 2008-05-14
// =============================================================================
class SelectionColorModifier : public ColorModifier_ABC
                             , public tools::Observer_ABC
                             , public tools::SelectionObserver< kernel::Entity_ABC >
                             , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             SelectionColorModifier( kernel::Controllers& controllers, GLView_ABC& tools, const kernel::Profile_ABC& profile );
    virtual ~SelectionColorModifier();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base ) const;
    virtual float Apply( const kernel::Entity_ABC& entity, float alpha ) const;
    //@}

    //! @name Static operations
    //@{
    static QColor SelectedColor( const QColor& base );
    static QColor SuperiorSelectedColor( const QColor& base );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifySelectionChanged( const T_Elements& elements );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    GLView_ABC& tools_;
    const kernel::Profile_ABC& profile_;
    T_Elements selectedElements_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

}

#endif // __SelectionColorModifier_h_
