// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoFuneralDialog_h_
#define __InfoFuneralDialog_h_

#include "InfoDialog.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoFuneralDialog
    @brief  InfoFuneralDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoFuneralDialog : public InfoDialog_Base
                        , public tools::Observer_ABC
                        , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoFuneralDialog( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoFuneralDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    kernel::Controllers& controllers_;
};

#endif // __InfoFuneralDialog_h_
