// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ExtensionsPanel_h_
#define __ExtensionsPanel_h_

#include "clients_gui/ExtensionsPanel.h"

// =============================================================================
/** @class  ExtensionsPanel
    @brief  ExtensionsPanel
*/
// Created: JSR 2011-12-08
// =============================================================================
class ExtensionsPanel : public gui::ExtensionsPanel
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionsPanel( QMainWindow* parent, kernel::Controllers& controllers, const kernel::ExtensionTypes& extensions,
                              const tools::Resolver< kernel::Agent_ABC >& agents, const tools::Resolver< kernel::Formation_ABC >& formations );
    virtual ~ExtensionsPanel();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OnChangeNationality();
    //@}
};

#endif // __ExtensionsPanel_h_
