// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPanel_h_
#define __ObjectPanel_h_

#include "clients_gui/ObjectPanel.h"

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: AGE 2006-09-08
// =============================================================================
class ObjectPanel : public gui::ObjectPanel
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ObjectPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ObjectPanel();
    //@}
};

#endif // __ObjectPanel_h_
