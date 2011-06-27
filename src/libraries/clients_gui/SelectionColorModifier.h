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
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class GlTools_ABC;
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
{
public:
    //! @name Constructors/Destructor
    //@{
             SelectionColorModifier( kernel::Controllers& controllers, kernel::GlTools_ABC& tools );
    virtual ~SelectionColorModifier();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    QColor SelectedColor( const QColor& base ) const;
    QColor SuperiorSelectedColor( const QColor& base ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::GlTools_ABC& tools_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    //@}
};

}

#endif // __SelectionColorModifier_h_
