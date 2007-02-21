// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoCompositionDialog_h_
#define __InfoCompositionDialog_h_

#include "InfoDialog.h"
#include "gaming/Equipments.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoCompositionDialog
    @brief  InfoCompositionDialog
*/
// Created: SBO 2007-02-19
// =============================================================================
class InfoCompositionDialog : public InfoDialog< Equipments >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoCompositionDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoCompositionDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint() const;
    //@}
};

#endif // __InfoCompositionDialog_h_
