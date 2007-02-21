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

#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <qiconview.h>

namespace kernel
{
    class Entity_ABC;
    class Controllers;
}

namespace gui
{
    class SymbolIcons;
}

// =============================================================================
/** @class  InfoSubordinatesWidget
    @brief  InfoSubordinatesWidget
*/
// Created: SBO 2007-02-21
// =============================================================================
class InfoSubordinatesWidget : public QIconView
                             , public kernel::Observer_ABC
                             , public kernel::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoSubordinatesWidget( QWidget* parent, kernel::Controllers& controllers, gui::SymbolIcons& icons );
    virtual ~InfoSubordinatesWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OpenItem( QIconViewItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfoSubordinatesWidget( const InfoSubordinatesWidget& );            //!< Copy constructor
    InfoSubordinatesWidget& operator=( const InfoSubordinatesWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    void AddSubordinate( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::SymbolIcons& icons_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __InfoSubordinatesWidget_h_
