// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalListView_h_
#define __TacticalListView_h_

#include "clients_kernel/TacticalHierarchies.h"
#include "clients_gui/HierarchyListView.h"

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
*/
// Created: AGE 2006-10-20
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~TacticalListView();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalListView( const TacticalListView& );            //!< Copy constructor
    TacticalListView& operator=( const TacticalListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __TacticalListView_h_
