// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfilePopulationRights_h_
#define __UserProfilePopulationRights_h_

#include "UserProfileRights_ABC.h"
#include "clients_gui/PopulationListView.h"

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
             UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~UserProfilePopulationRights();
    //@}

    //! @name Operations
    //@{
    virtual bool NeedSaving() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( QListViewItem*, const QPoint&, int );
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
    //@}
};

#endif // __UserProfilePopulationRights_h_
