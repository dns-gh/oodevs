// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticStatusWidgets.h"
#include "clients_gui/BaseDisplayer.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceStatusWidget::MaintenanceStatusWidget( QWidget* parent, kernel::Controllers& controllers )
    : LogisticStatusWidget_ABC< kernel::MaintenanceStates_ABC >( "Maintenance status widget", parent, controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceStatusWidget::~MaintenanceStatusWidget()
{
    // NOTHING
}

namespace
{
    template< typename T >
    struct DisplayerAdapter : gui::BaseDisplayer
    {
        DisplayerAdapter( T& table )
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

        T& table_;
        QString name_;
        QString content_;
    };
}

void MaintenanceStatusWidget::OnUpdated( const kernel::MaintenanceStates_ABC& states )
{
    DisplayerAdapter< MaintenanceStatusWidget > displayer( *this );
    states.Display( displayer );
}

void MaintenanceStatusWidget::OnUpdated( const gui::LogisticBase& base )
{
    Add( tools::translate( "MaintenanceStatusWidget", "Mode" ), base.IsMaintenanceManual()
         ? tools::translate( "MaintenanceStatusWidget", "Manual" )
         : tools::translate( "MaintenanceStatusWidget", "Automatic" ) );
}

void MaintenanceStatusWidget::Update()
{
    dataModel_->clear();
    if( const auto* extension = selected_->Retrieve< gui::LogisticBase >() )
        OnUpdated( *extension );
    if( const auto* extension = selected_->Retrieve< kernel::MaintenanceStates_ABC >() )
        OnUpdated( *extension );
}

void MaintenanceStatusWidget::NotifyUpdated( const kernel::MaintenanceStates_ABC& states )
{
    if( selected_ && selected_->Retrieve< kernel::MaintenanceStates_ABC >() == &states )
        Update();
}

void MaintenanceStatusWidget::NotifyUpdated( const gui::LogisticBase& base )
{
    if( selected_ && selected_->Retrieve< gui::LogisticBase >() == &base )
        Update();
}

void MaintenanceStatusWidget::NotifySelected( const kernel::Entity_ABC* element )
{
    selected_ = element;
    if( !selected_ )
        return;
    if( selected_->Retrieve< kernel::MaintenanceStates_ABC >() ||
        selected_->Retrieve< gui::LogisticBase >() )
    {
        show();
        Update();
    }
    else
        hide();
}

// -----------------------------------------------------------------------------
// Name: MedicalStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalStatusWidget::MedicalStatusWidget( QWidget* parent, kernel::Controllers& controllers )
    : LogisticStatusWidget_ABC< MedicalStates >( "Medical status widget", parent, controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MedicalStatusWidget::~MedicalStatusWidget()
{
    // NOTHING
}

void MedicalStatusWidget::OnUpdated( const MedicalStates& states )
{
    DisplayerAdapter< MedicalStatusWidget > displayer( *this );
    states.Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: SupplyStatusWidget constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyStatusWidget::SupplyStatusWidget( QWidget* parent, kernel::Controllers& controllers )
    : LogisticStatusWidget_ABC< SupplyStates >( "Supply status widget", parent, controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStatusWidget destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
SupplyStatusWidget::~SupplyStatusWidget()
{
    // NOTHING
}

void SupplyStatusWidget::OnUpdated( const SupplyStates& states )
{
    DisplayerAdapter< SupplyStatusWidget > displayer( *this );
    states.Display( displayer );
}
