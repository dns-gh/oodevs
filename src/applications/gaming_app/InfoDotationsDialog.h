// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoDotationsDialog_h_
#define __InfoDotationsDialog_h_

#include "InfoDialog.h"
#include "clients_kernel/Dotations_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoDotationsDialog
    @brief  InfoDotationsDialog
    // $$$$ SBO 2008-07-08: Ordnance seems to be more correct than Dotations (which doesn't exists...)
*/
// Created: SBO 2007-02-19
// =============================================================================
class InfoDotationsDialog : public InfoDialog< kernel::Dotations_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoDotationsDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoDotationsDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    //@}
};

#endif // __InfoDotationsDialog_h_
