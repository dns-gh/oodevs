// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticStatusWidget_ABC_h_
#define __LogisticStatusWidget_ABC_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/DisplayBuilder.h"
#include "clients_kernel/Entity_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class DisplayBuilder;
}

// =============================================================================
/** @class  LogisticStatusWidget_ABC
    @brief  LogisticStatusWidget_ABC
*/
// Created: SBO 2007-02-19
// =============================================================================
template< typename Extension >
class LogisticStatusWidget_ABC : public Q3VBox
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< Extension >
                               , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticStatusWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~LogisticStatusWidget_ABC();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticStatusWidget_ABC( const LogisticStatusWidget_ABC& );            //!< Copy constructor
    LogisticStatusWidget_ABC& operator=( const LogisticStatusWidget_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Extension& element );
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}

protected:
    //! @name Member data
    //@{
    gui::DisplayBuilder* display_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget_ABC constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Extension >
LogisticStatusWidget_ABC< Extension >::LogisticStatusWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : Q3VBox( parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    display_ = new gui::DisplayBuilder( this, factory );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget_ABC destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Extension >
LogisticStatusWidget_ABC< Extension >::~LogisticStatusWidget_ABC()
{
    controllers_.Unregister( *this );
    delete display_;
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget_ABC::showEvent
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget_ABC< Extension >::showEvent( QShowEvent* )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget_ABC::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget_ABC< Extension >::NotifyUpdated( const Extension& element )
{
    if( selected_ && selected_->Retrieve< Extension >() == &element )
        element.Display( *display_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget_ABC::NotifySelected
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget_ABC< Extension >::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    if( !selected_ )
        return;
    if( const Extension* extension = selected_->Retrieve< Extension >() )
    {
        show();
        NotifyUpdated( *extension );
    }
    else
        hide();
}

#endif // __LogisticStatusWidget_ABC_h_
