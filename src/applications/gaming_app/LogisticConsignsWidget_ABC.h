// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticConsignsWidget_ABC_h_
#define __LogisticConsignsWidget_ABC_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/SubItemDisplayer.h"
#include "gaming/LogisticConsigns.h"
#include "gaming/Tools.h"

namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  LogisticConsignsWidget_ABC
    @brief  LogisticConsignsWidget_ABC
*/
// Created: SBO 2007-02-19
// =============================================================================
template< typename ConcreteDisplayer, typename Consign, typename Extension >
class LogisticConsignsWidget_ABC : public Q3VBox
                                 , public tools::Observer_ABC
                                 , public tools::ElementObserver_ABC< Extension >
                                 , public tools::ElementObserver_ABC< Consign >
                                 , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticConsignsWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~LogisticConsignsWidget_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const Consign* consign, kernel::Displayer_ABC& displayer, gui::ValuedListItem* );
    //@}

protected:
    //! @name Operations
    //@{
    void AddConsignColumn( const QString& column );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticConsignsWidget_ABC( const LogisticConsignsWidget_ABC& );            //!< Copy constructor
    LogisticConsignsWidget_ABC& operator=( const LogisticConsignsWidget_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* );
    virtual void NotifyUpdated( const Extension& consigns );
    virtual void NotifyUpdated( const Consign& consigns );
    virtual void NotifySelected( const kernel::Entity_ABC* agent );

    virtual void DisplayRequested( const Extension& consigns, gui::ListDisplayer< ConcreteDisplayer >* list );
    virtual void DisplayHandled( const Extension& consigns, gui::ListDisplayer< ConcreteDisplayer >* list );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    gui::ListDisplayer< ConcreteDisplayer >* pConsignListView_;
    gui::ListDisplayer< ConcreteDisplayer >* pConsignHandledListView_;
    gui::SubItemDisplayer* logDisplay_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::LogisticConsignsWidget_ABC( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : Q3VBox( parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    pConsignListView_ = new gui::ListDisplayer< ConcreteDisplayer >( this, (ConcreteDisplayer&)*this, factory );
    pConsignListView_->AddColumn( tools::translate( "LogisticConsignsWidget_ABC", "Logistic requests" ) );
    pConsignListView_->AddColumn( "" );
    pConsignListView_->setResizeMode( Q3ListView::AllColumns );

    pConsignHandledListView_ = new gui::ListDisplayer< ConcreteDisplayer >( this, (ConcreteDisplayer&)*this, factory );
    pConsignHandledListView_->AddColumn( tools::translate( "LogisticConsignsWidget_ABC", "Processing consigns" ) );
    pConsignHandledListView_->AddColumn( "" );
    pConsignHandledListView_->setResizeMode( Q3ListView::AllColumns );

    logDisplay_ = new gui::SubItemDisplayer( tools::translate( "LogisticConsignsWidget_ABC", "Consign:" ), factory );
    logDisplay_->AddChild( tools::translate( "LogisticConsignsWidget_ABC", "Instruction:" ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::~LogisticConsignsWidget_ABC()
{
    controllers_.Unregister( *this );
    delete logDisplay_;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::Display
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::Display( const Consign* consign, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item )
{
    if( consign )
    {
        item->SetValue( consign );
        consign->Display( displayer, (*logDisplay_)( item ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::AddConsignColumn
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::AddConsignColumn( const QString& column )
{
    logDisplay_->AddChild( column );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::showEvent
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* entity = selected_;
    selected_ = 0;
    NotifySelected( entity );
    Q3VBox::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::NotifyUpdated( const Extension& consigns )
{
    if( selected_ && selected_->Retrieve< Extension >() == &consigns )
    {
        DisplayRequested( consigns, pConsignListView_ );
        DisplayHandled( consigns, pConsignHandledListView_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::NotifyUpdated( const Consign& consign )
{
    gui::ValuedListItem* item = gui::FindItem( &consign, pConsignListView_->firstChild() );
    if( item )
        consign.Display( pConsignListView_->GetItemDisplayer( item ), (*logDisplay_)( item ) );
    else
    {
        item = gui::FindItem( &consign, pConsignHandledListView_->firstChild() );
        if( item )
            consign.Display( pConsignHandledListView_->GetItemDisplayer( item ), (*logDisplay_)( item ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifySelected
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    if( const Extension* extension = selected_ ? selected_->Retrieve< Extension >() : 0 )
    {
        show();
        NotifyUpdated( *extension );
    }
    else
        hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::DisplayRequested
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::DisplayRequested( const Extension& consigns, gui::ListDisplayer< ConcreteDisplayer >* list )
{
    list->DeleteTail(
        list->DisplayList( consigns.requested_.begin(), consigns.requested_.end() )
        );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::DisplayHandled
// Created: SBO 2007-03-30
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::DisplayHandled( const Extension& consigns, gui::ListDisplayer< ConcreteDisplayer >* list )
{
    list->DeleteTail(
        list->DisplayList( consigns.handled_.begin(), consigns.handled_.end() )
        );
}

#endif // __LogisticConsignsWidget_ABC_h_
