// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoConflictsTab_h_
#define __InfoConflictsTab_h_

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  InfoConflictsTab
    @brief  InfoConflictsTab
*/
// Created: SBO 2007-02-15
// =============================================================================
class InfoConflictsTab : public QVBox
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoConflictsTab( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~InfoConflictsTab();
    //@}
};

#endif // __InfoConflictsTab_h_
