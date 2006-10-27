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
    class EditorFactory_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
}

class Model;
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
             InfoPanels( QWidget* parent, kernel::Controllers& controllers, Model& model, const StaticModel& staticModel, gui::ItemFactory_ABC& factory );
    virtual ~InfoPanels();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    InfoPanels( const InfoPanels& );            //!< Copy constructor
    InfoPanels& operator=( const InfoPanels& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::EditorFactory_ABC& editorFactory_;
    //@}
};

#endif // __InfoPanels_h_
