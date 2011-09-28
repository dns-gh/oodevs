// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileUnitControls_h_
#define __UserProfileUnitControls_h_

#include "clients_gui/HierarchyListView.h"
#include "preparation/ProfileHierarchies_ABC.h"
#include "UserProfileControls_ABC.h"

class Model;

// =============================================================================
/** @class  UserProfileUnitControls
    @brief  User profile unit controls
*/
// Created: LGY 2011-09-13
// =============================================================================
class UserProfileUnitControls : public gui::HierarchyListView< ProfileHierarchies_ABC >
                              , public UserProfileControls_ABC
                              , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileUnitControls( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory,
                                      gui::EntitySymbols& icons, ControlsChecker_ABC& checker, Model& model );
    virtual ~UserProfileUnitControls();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    void Show();
    //@}

signals:
    //! @name signals
    //@{
    void ProfiledChanged( const kernel::Entity_ABC* entity, bool isReadable, bool isWriteable );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnProfiledChanged( const kernel::Entity_ABC* entity, bool isWriteable );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Operations
    //@{
    virtual void ValueChanged( const kernel::Entity_ABC* entity, bool isReadable, bool isWriteable );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member Data
    //@{
Model& model_;
    //@}
};

#endif // __UserProfileUnitControls_h_
