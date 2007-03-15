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

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_gui/SubItemDisplayer.h"
#include "gaming/LogisticConsigns.h"

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
class LogisticConsignsWidget_ABC : public QVBox
                                 , public kernel::Observer_ABC
                                 , public kernel::ElementObserver_ABC< LogisticConsigns >
                                 , public kernel::ElementObserver_ABC< Consign >
                                 , public kernel::SelectionObserver< kernel::Entity_ABC >
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
    virtual void NotifyUpdated( const LogisticConsigns& consigns );
    virtual void NotifyUpdated( const Consign& consigns );
    virtual void NotifySelected( const kernel::Entity_ABC* agent );

    virtual void DisplayRequested( const LogisticConsigns& consigns, gui::ListDisplayer< ConcreteDisplayer >* list ) = 0;
    virtual void DisplayHandled( const LogisticConsigns& consigns, gui::ListDisplayer< ConcreteDisplayer >* list ) = 0;
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
    : QVBox( parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    pConsignListView_ = new gui::ListDisplayer< ConcreteDisplayer >( this, (ConcreteDisplayer&)*this, factory );
    pConsignListView_->AddColumn( tr( "Logistic requests" ) );
    pConsignListView_->AddColumn( "" );
    pConsignListView_->setResizeMode( QListView::AllColumns );

    pConsignHandledListView_ = new gui::ListDisplayer< ConcreteDisplayer >( this, (ConcreteDisplayer&)*this, factory );
    pConsignHandledListView_->AddColumn( tr( "Processing consigns" ) );
    pConsignHandledListView_->AddColumn( "" );
    pConsignHandledListView_->setResizeMode( QListView::AllColumns );

    logDisplay_ = new gui::SubItemDisplayer( tr( "Consign:" ), factory );
    logDisplay_->AddChild( tr( "Instruction:" ) )
                .AddChild( tr( "Consumer:" ) )
                .AddChild( tr( "Handler:" ) );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::~LogisticConsignsWidget_ABC()
{
    controllers_.Remove( *this );
    delete logDisplay_;
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::Display
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::Display( const Consign* consign, kernel::Displayer_ABC&, gui::ValuedListItem* item )
{
    if( consign )
    {
        item->SetValue( consign );
        consign->Display( (*logDisplay_)( item ) );
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
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::showEvent( QShowEvent* )
{
    const kernel::Entity_ABC* entity = selected_;
    selected_ = 0;
    NotifySelected( entity );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::NotifyUpdated( const LogisticConsigns& consigns )
{
    if( selected_ && selected_->Retrieve< LogisticConsigns >() == &consigns )
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
    if( ! item )
        item = gui::FindItem( &consign, pConsignHandledListView_->firstChild() );
    if( item )
        consign.Display( (*logDisplay_)( item ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticConsignsWidget_ABC::NotifySelected
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Consign, typename Extension >
void LogisticConsignsWidget_ABC< ConcreteDisplayer, Consign, Extension >::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    if( !selected_  || !selected_->Retrieve< Extension >() )
    {
        hide();
        return;
    }
    if( const LogisticConsigns* extension = selected_->Retrieve< LogisticConsigns >() )
    {
        show();
        NotifyUpdated( *extension );
    }
    else
        hide();
}

#endif // __LogisticConsignsWidget_ABC_h_
