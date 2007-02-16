// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoReportsTab_h_
#define __InfoReportsTab_h_

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoReportsTab
    @brief  InfoReportsTab
*/
// Created: SBO 2007-02-06
// =============================================================================
class InfoReportsTab : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoReportsTab( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoReportsTab();
    //@}
};

#endif // __InfoReportsTab_h_
