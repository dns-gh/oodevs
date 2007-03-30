// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoSupplyDialog_h_
#define __InfoSupplyDialog_h_

#include "InfoDialog.h"
#include "gaming/SupplyStates.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoSupplyDialog
    @brief  InfoSupplyDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoSupplyDialog : public InfoDialog< SupplyStates >
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoSupplyDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoSupplyDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    //@}
};

#endif // __InfoSupplyDialog_h_
