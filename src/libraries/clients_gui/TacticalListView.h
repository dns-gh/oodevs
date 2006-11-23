// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_TacticalListView_h_
#define __gui_TacticalListView_h_

#include "clients_kernel/TacticalHierarchies.h"
#include "HierarchyListView.h"

namespace gui
{
    class SymbolIcons;

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
*/
// Created: AGE 2006-11-22
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::SymbolIcons& icons );
    virtual ~TacticalListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalListView( const TacticalListView& );            //!< Copy constructor
    TacticalListView& operator=( const TacticalListView& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    gui::SymbolIcons& icons_;
    QTimer* timer_;
    //@}
};

}

#endif // __gui_TacticalListView_h_
