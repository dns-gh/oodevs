// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfilePopulationControls_h_
#define __UserProfilePopulationControls_h_

#include "UserProfileControls_ABC.h"
#include "clients_gui/PopulationListView.h"

class ControlsChecker_ABC;

// =============================================================================
/** @class  UserProfilePopulationControls
    @brief  User profile population controls
*/
// Created: LGY 2011-09-12
// =============================================================================
class UserProfilePopulationControls : public gui::PopulationListView
                                    , public UserProfileControls_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfilePopulationControls( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                            ControlsChecker_ABC& checker );
    virtual ~UserProfilePopulationControls();

    virtual void showEvent( QShowEvent* event );
    virtual void hideEvent( QHideEvent* event );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Operation
    //@{
    virtual Q3DragObject* dragObject();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    virtual bool CanWrite( const kernel::Entity_ABC* entity ) const;
    virtual void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );
    //@}
};

#endif // __UserProfilePopulationControls_h_
