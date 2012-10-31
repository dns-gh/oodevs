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
    class DisplayExtractor_ABC;
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
             InfoMedicalDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, kernel::DisplayExtractor_ABC& extractor );
    virtual ~InfoMedicalDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    //@}
};

#endif // __InfoMedicalDialog_h_
