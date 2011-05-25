// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoReportsTab.h"
#include "ReportListView.h"
#include "ReportFilterOptions.h"
#include "clients_gui/ItemFactory_ABC.h"
#include "gaming/Reports.h"

namespace
{
    class AlternatingItemFactory : public gui::ItemFactory_ABC
    {
    public:
        explicit AlternatingItemFactory( gui::ItemFactory_ABC& factory )
            : factory_( factory )
            , color1_ ( Qt::white )
            , color2_ ( QColor( 240, 240, 240 ) )
        {
        }
        virtual ~AlternatingItemFactory() {}
        virtual gui::ValuedListItem* CreateItem( QListView* parent )
        {
            gui::ValuedListItem* item = factory_.CreateItem( parent );
            item->SetBackgroundColor( color1_, color2_ );
            return item;
        }
        virtual gui::ValuedListItem* CreateItem( QListViewItem* parent )
        {
            gui::ValuedListItem* item = factory_.CreateItem( parent );
            item->SetBackgroundColor( color1_, color2_ );
            return item;
        }
        virtual gui::ValuedListItem* CreateItem( QListView* parent, QListViewItem* after )
        {
            gui::ValuedListItem* item = factory_.CreateItem( parent, after );
            item->SetBackgroundColor( color1_, color2_ );
            return item;
        }
        virtual gui::ValuedListItem* CreateItem( QListViewItem* parent, QListViewItem* after )
        {
            gui::ValuedListItem* item = factory_.CreateItem( parent, after );
            item->SetBackgroundColor( color1_, color2_ );
            return item;
        }
        virtual gui::RichLabel* CreateLabel( QWidget*, const char* ) { throw std::runtime_error( __FUNCTION__ " not implemented" ); }
        virtual gui::RichLabel* CreateLabel( const QString&, QWidget*, const char* ) { throw std::runtime_error( __FUNCTION__ " not implemented" ); }
        virtual gui::RichLabel* CreateLabel( const QString&, bool, QWidget*, const char* ) { throw std::runtime_error( __FUNCTION__ " not implemented" ); }

        gui::ItemFactory_ABC& factory_;
        const QColor color1_;
        const QColor color2_;
    };
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab constructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::InfoReportsTab( QTabWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QVBox( parent, "InfoReportsTab" )
    , controllers_( controllers )
    , parent_( parent )
    , factory_( new AlternatingItemFactory( factory ) )
{
    setMargin( 0 );
    ReportFilterOptions* filter  = new ReportFilterOptions( this );
    ReportListView*      reports = new ReportListView( this, controllers, *filter, *factory_ );
    filter->hide();
    reports->header()->hide();
    connect( filter, SIGNAL( OptionsChanged() ), reports, SLOT( OnOptionsChanged() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab destructor
// Created: SBO 2007-02-06
// -----------------------------------------------------------------------------
InfoReportsTab::~InfoReportsTab()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoReportsTab::NotifySelected
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void InfoReportsTab::NotifySelected( const kernel::Entity_ABC* entity )
{
    int current = parent_->currentPageIndex();
    parent_->setTabEnabled( this, entity && entity->Retrieve< Reports >() );
    parent_->setCurrentPage( current );
}
