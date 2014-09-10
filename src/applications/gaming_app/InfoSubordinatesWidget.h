// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfoSubordinatesWidget_h_
#define __InfoSubordinatesWidget_h_

#include "clients_kernel/SafePointer.h"
#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class EntitySymbols;
}

// =============================================================================
/** @class  InfoSubordinatesWidget
    @brief  InfoSubordinatesWidget
*/
// Created: SBO 2007-02-21
// =============================================================================
class InfoSubordinatesWidget : public QListWidget
                             , public tools::Observer_ABC
                             , public tools::SelectionObserver< kernel::Entity_ABC >
                             , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             InfoSubordinatesWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, const gui::EntitySymbols& icons );
    virtual ~InfoSubordinatesWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OpenItem( QListWidgetItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    void AddSubordinate( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const gui::EntitySymbols& icons_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __InfoSubordinatesWidget_h_
