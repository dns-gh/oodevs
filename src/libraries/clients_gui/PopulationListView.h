// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef gui_PopulationListView_h
#define gui_PopulationListView_h

#include "EntityListView.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Population_ABC;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  PopulationListView
    @brief  Population list view
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public EntityListView
                         , public tools::ElementObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~PopulationListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Population_ABC& population );
    virtual void NotifyDeleted( const kernel::Population_ABC& population );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

}

#endif // gui_PopulationListView_h
