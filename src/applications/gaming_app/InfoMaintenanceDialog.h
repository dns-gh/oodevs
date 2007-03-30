// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoMaintenanceDialog_h_
#define __InfoMaintenanceDialog_h_

#include "InfoDialog.h"
#include "gaming/MaintenanceStates.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoMaintenanceDialog
    @brief  InfoMaintenanceDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoMaintenanceDialog : public InfoDialog< MaintenanceStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoMaintenanceDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoMaintenanceDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    //@}
};

#endif // __InfoMaintenanceDialog_h_
