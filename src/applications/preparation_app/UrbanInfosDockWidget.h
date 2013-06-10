// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __UrbanInfosDockWidget_h_
#define __UrbanInfosDockWidget_h_

#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/MultipleSelectionObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class UrbanObject_ABC;
}

namespace gui
{
    template< typename T > class RichWidget;
}

class StaticModel;
class UrbanModel;

// =============================================================================
/** @class  UrbanInfosDockWidget
    @brief  UrbanInfosDockWidget
*/
// Created: ABR 2012-05-16
// =============================================================================
class UrbanInfosDockWidget : public gui::RichDockWidget
                           , public kernel::MultipleSelectionObserver< kernel::UrbanObject_ABC >
                           , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanInfosDockWidget( QWidget* parent, kernel::Controllers& controllers, UrbanModel& model );
    virtual ~UrbanInfosDockWidget();
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifySelectionChanged( const T_Elements& elements );
    virtual void NotifyCreated( const kernel::UrbanObject_ABC& element );
    virtual void NotifyDeleted( const kernel::UrbanObject_ABC& element );
    virtual void NotifyUpdated( const kernel::UrbanObject_ABC& element );
    //@}

private:
    //! @name Helpers
    //@{
    void InsertLine( const QString& line = "", const QString& value = "" );
    void Update();
    void SelectBlocks( const kernel::UrbanObject_ABC& urbanObject );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    UrbanModel&          model_;
    QLabel*              urbanBlocksCount_;
    gui::RichWidget< QTextEdit >*   infoEditText_;
    QStringList          infos_;
    QStringList          values_;
    T_Elements           selectedElements_;
    //@}
};

#endif // __UrbanInfosDockWidget_h_
