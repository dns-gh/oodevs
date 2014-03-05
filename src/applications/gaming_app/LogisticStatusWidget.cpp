// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticStatusWidget.h"

#include "clients_gui/BaseDisplayer.h"
#include "clients_gui/CommonDelegate.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/MaintenanceStates.h"
#include "gaming/MedicalStates.h"
#include "gaming/SupplyStates.h"

namespace
{
    template< typename Extension >
    struct DisplayerAdapter : gui::BaseDisplayer
    {
        DisplayerAdapter( LogisticStatusWidget< Extension >& table )
            : table_( table )
        {}
        virtual kernel::Displayer_ABC& Group( const QString& )
        {
            return *this;
        }
        virtual void Clear()
        {}
        virtual void Hide()
        {}
        virtual kernel::Displayer_ABC& SubItem( const QString& name )
        {
            name_ = name;
            content_.clear();
            return *this;
        }
        virtual void StartDisplay()
        {}
        virtual void DisplayFormatted( const QString& formatted )
        {
            if( formatted == ", " )
                name_.clear();
            else
                table_.Add( name_, formatted );
        }
        virtual void EndDisplay()
        {
            name_.clear();
        }

        LogisticStatusWidget< Extension >& table_;
        QString name_;
        QString content_;
    };
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget constructor
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
LogisticStatusWidget< Extension >::LogisticStatusWidget( const QString& objectName,
                                                         QWidget* parent,
                                                         kernel::Controllers& controllers,
                                                         const T_LogisticBaseModeChecker& modeChecker )
    : gui::RichWidget< QTableView >( objectName, parent )
    , controllers_( controllers )
    , selected_( controllers )
    , modeChecker_( modeChecker )
{
    dataModel_ = new QStandardItemModel( this );
    proxyModel_= new QSortFilterProxyModel( this );
    delegate_  = new gui::CommonDelegate( this );
    dataModel_->setColumnCount( 2 );
    proxyModel_->setSourceModel( dataModel_ );
    proxyModel_->setSortRole( Qt::UserRole );
    setModel( proxyModel_ );
    setItemDelegate( delegate_ );
    setSortingEnabled( false );
    setShowGrid( false );
    setAlternatingRowColors( false );
    horizontalHeader()->setVisible( false );
    verticalHeader()->setVisible( false );
    setSelectionMode( NoSelection );
    setEditTriggers( NoEditTriggers );
    setFrameStyle( QFrame::NoFrame );
    setFocusPolicy( Qt::NoFocus );
    horizontalHeader()->setResizeMode( QHeaderView::Stretch );
    QPalette p = palette();
    p.setColor( QPalette::Base, palette().background().color() );
    setPalette( p );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget destructor
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
LogisticStatusWidget< Extension >::~LogisticStatusWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget::Add
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget< Extension >::Add( const QString& title, const QString& value )
{
    const int row = dataModel_->rowCount();
    QStandardItem* itemTitle = new QStandardItem();
    itemTitle->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    QFont curFont = itemTitle->font();
    curFont.setBold( true );
    curFont.setWeight( curFont.weight() - 2 );
    itemTitle->setFont( curFont );
    itemTitle->setData( QVariant( title ), Qt::DisplayRole );
    dataModel_->setItem( row, 0, itemTitle );

    QStandardItem* itemValue = new QStandardItem();
    itemValue->setTextAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    itemValue->setData( QVariant( value ), Qt::DisplayRole );
    dataModel_->setItem( row, 1, itemValue );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget::showEvent
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget< Extension >::showEvent( QShowEvent* )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget::NotifyUpdated
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget< Extension >::NotifyUpdated( const Extension& extension )
{
    if( selected_ && selected_->Retrieve< Extension >() == &extension )
        Update();
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget::NotifyUpdated
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget< Extension >::NotifyUpdated( const gui::LogisticBase& base )
{
    if( selected_ && modeChecker_ && selected_->Retrieve< gui::LogisticBase >() == &base )
        Update();
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget::Update
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget< Extension >::Update()
{
    dataModel_->clear();
    if( const auto* extension = selected_->Retrieve< Extension >() )
    {
        DisplayerAdapter< Extension > displayer( *this );
        extension->Display( displayer );
    }
    if( const auto* logisticBase = selected_->Retrieve< gui::LogisticBase >() )
        if( logisticBase->IsBase() && modeChecker_ )
            Add( tools::translate( "LogisticStatusWidget", "Mode" ), modeChecker_( *logisticBase )
                    ? tools::translate( "LogisticStatusWidget", "Manual" )
                    : tools::translate( "LogisticStatusWidget", "Automatic" ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticStatusWidget::NotifySelected
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticStatusWidget< Extension >::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    if( !selected_ )
        return;
    if( selected_->Retrieve< Extension >() ||
        modeChecker_ && selected_->Retrieve< gui::LogisticBase >() )
    {
        show();
        Update();
    }
    else
        hide();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStatusWidget constructor
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
MaintenanceStatusWidget::MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers )
        : LogisticStatusWidget< kernel::MaintenanceStates_ABC >( "maintenance_status_widget",
                                                                 parent,
                                                                 controllers,
                                                                 []( const gui::LogisticBase& base ){ return base.IsMaintenanceManual(); } )
    {}

// -----------------------------------------------------------------------------
// Name: MedicalStatusWidget constructor
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
MedicalStatusWidget::MedicalStatusWidget( QWidget* parent, kernel::Controllers& controllers )
        : LogisticStatusWidget< MedicalStates >( "medical_status_widget",
                                                 parent,
                                                 controllers )
    {}

// -----------------------------------------------------------------------------
// Name: SupplyStatusWidget constructor
// Created: ABR 2014-03-03
// -----------------------------------------------------------------------------
SupplyStatusWidget::SupplyStatusWidget( QWidget* parent, kernel::Controllers& controllers )
        : LogisticStatusWidget< SupplyStates >( "supply_status_widget",
                                                parent,
                                                controllers,
                                                []( const gui::LogisticBase& base ){ return base.IsSupplyManual(); } )
    {}
