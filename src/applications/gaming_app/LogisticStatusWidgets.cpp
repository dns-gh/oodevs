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
#include "clients_kernel/Displayer_ABC.h"
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
    struct DisplayerAdapter : kernel::Displayer_ABC
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
