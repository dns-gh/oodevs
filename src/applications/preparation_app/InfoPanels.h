// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoPanels_h_
#define __InfoPanels_h_

#include "clients_gui/Panels.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

class StaticModel;

// =============================================================================
/** @class  InfoPanels
    @brief  InfoPanels
*/
// Created: SBO 2006-08-28
// =============================================================================
class InfoPanels : public gui::Panels
{
public:
    //! @name Constructors/Destructor
    //@{
             InfoPanels( QWidget* parent, kernel::Controllers& controllers, const StaticModel& model, gui::ItemFactory_ABC& factory );
    virtual ~InfoPanels();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    InfoPanels( const InfoPanels& );            //!< Copy constructor
    InfoPanels& operator=( const InfoPanels& ); //!< Assignement operator
    //@}
};

#endif // __InfoPanels_h_
