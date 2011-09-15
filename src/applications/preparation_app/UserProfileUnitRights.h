// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileUnitRights_h_
#define __UserProfileUnitRights_h_

#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "UserProfileRights_ABC.h"

namespace
{
    typedef gui::HierarchyListView< kernel::TacticalHierarchies > T_Parent;
}

// =============================================================================
/** @class  UserProfileUnitRights
    @brief  UserProfileUnitRights
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileUnitRights : public ::T_Parent
                            , public UserProfileRights_ABC
                            , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileUnitRights( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons );
    virtual ~UserProfileUnitRights();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    //@}

signals:
    //! @name signals
    //@{
    void ProfiledChanged( const kernel::Entity_ABC* entity, bool isWriteable );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnProfiledChanged( const kernel::Entity_ABC* entity, bool isReadable, bool isWriteable );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnItemClicked( Q3ListViewItem*, const QPoint&, int );
    //@}

private:
    //! @name Operations
    //@{
    virtual void ValueChanged( const kernel::Entity_ABC* entity, bool isWriteable );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileUnitRights( const UserProfileUnitRights& );            //!< Copy constructor
    UserProfileUnitRights& operator=( const UserProfileUnitRights& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    //@}
};

#endif // __UserProfileUnitRights_h_
