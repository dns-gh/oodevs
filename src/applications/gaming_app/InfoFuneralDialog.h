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
#include "clients_kernel/SafePointer.h"
#include "gaming/Troops.h"

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class DisplayExtractor;
}

class Publisher_ABC;

// =============================================================================
/** @class  InfoFuneralDialog
    @brief  InfoFuneralDialog
*/
// Created: SBO 2007-02-20
// =============================================================================
class InfoFuneralDialog : public InfoDialog_Base
                        , public tools::Observer_ABC
                        , public tools::SelectionObserver< kernel::Entity_ABC >
                        , public tools::ElementObserver_ABC< Troops >
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoFuneralDialog( QWidget* parent, kernel::Controllers& controllers
                              , gui::DisplayExtractor& extractor, const kernel::Profile_ABC& profile, Publisher_ABC& publisher );
    virtual ~InfoFuneralDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool ShouldDisplay( const kernel::Entity_ABC& element ) const;
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const Troops& troops );
    //@}

private:
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
};

#endif // __InfoFuneralDialog_h_
