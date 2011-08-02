// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfilePopulationRights_h_
#define __UserProfilePopulationRights_h_

#include "UserProfileRights_ABC.h"
#include "clients_gui/PopulationListView.h"

namespace kernel
{
    class Profile_ABC;
}

// =============================================================================
/** @class  UserProfilePopulationRights
    @brief  UserProfilePopulationRights
*/
// Created: SBO 2007-01-18
// =============================================================================
class UserProfilePopulationRights : public gui::PopulationListView
                                  , public UserProfileRights_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~UserProfilePopulationRights();
    //@}

    //! @name Operations
    //@{
    virtual bool NeedsCommit() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfilePopulationRights( const UserProfilePopulationRights& );            //!< Copy constructor
    UserProfilePopulationRights& operator=( const UserProfilePopulationRights& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    virtual void showEvent( QShowEvent* event );
    virtual void hideEvent( QHideEvent* event );
    //@}
};

#endif // __UserProfilePopulationRights_h_
