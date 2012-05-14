// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FileToolbar_h_
#define __FileToolbar_h_

#include "clients_gui/RichToolBar.h"

namespace kernel
{
    class Controllers;
}

class Menu;

// =============================================================================
/** @class  FileToolbar
    @brief  FileToolbar
*/
// Created: SBO 2006-12-13
// =============================================================================
class FileToolbar : public gui::RichToolBar
{

public:
    //! @name Constructors/Destructor
    //@{
             FileToolbar( QWidget* parent, kernel::Controllers& controllers, Menu& menu );
    virtual ~FileToolbar();
    //@}
};

#endif // __FileToolbar_h_
