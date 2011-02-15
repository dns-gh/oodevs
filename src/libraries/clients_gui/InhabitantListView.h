// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef gui_InhabitantListView_h
#define gui_InhabitantListView_h

#include "EntityListView.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Inhabitant_ABC;
    class Profile_ABC;
}

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  InhabitantListView
    @brief  Inhabitant list view
*/
// Created: HME 2005-10-03
// =============================================================================
class InhabitantListView : public EntityListView
                         , public tools::ElementObserver_ABC< kernel::Inhabitant_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantListView( QWidget* pParent, kernel::Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~InhabitantListView();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Inhabitant_ABC& inhabitant );
    virtual void NotifyDeleted( const kernel::Inhabitant_ABC& inhabitant );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ItemFactory_ABC& factory_;
    const kernel::Profile_ABC& profile_;
    //@}
};

}

#endif // gui_InhabitantListView_h
