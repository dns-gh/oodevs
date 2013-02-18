// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InfoPanels_h_
#define __InfoPanels_h_

#include "clients_gui/RichDockWidget.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class DisplayExtractor;
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoPanels
    @brief  Info panels container
*/
// Created: SBO 2006-08-08
// =============================================================================
class InfoPanels : public gui::RichDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoPanels( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::DisplayExtractor& extractor );
    virtual ~InfoPanels();
    //@}
};

#endif // __InfoPanels_h_
