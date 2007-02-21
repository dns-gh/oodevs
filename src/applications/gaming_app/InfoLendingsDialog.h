// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoLendingsDialog_h_
#define __InfoLendingsDialog_h_

#include "InfoDialog.h"
#include "gaming/Lendings.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoLendingsDialog
    @brief  InfoLendingsDialog
*/
// Created: SBO 2007-02-19
// =============================================================================
class InfoLendingsDialog : public InfoDialog< Lendings >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoLendingsDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoLendingsDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    //@}
};

#endif // __InfoLendingsDialog_h_
