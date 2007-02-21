// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMedicalDialog_h_
#define __InfoMedicalDialog_h_

#include "InfoDialog.h"
#include "gaming/MedicalStates.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoMedicalDialog
    @brief  InfoMedicalDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoMedicalDialog : public InfoDialog< MedicalStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoMedicalDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoMedicalDialog();
    //@}
};

#endif // __InfoMedicalDialog_h_
