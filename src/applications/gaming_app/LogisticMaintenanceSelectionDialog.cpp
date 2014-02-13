// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticMaintenanceSelectionDialog.h"
#include "moc_LogisticMaintenanceSelectionDialog.cpp"
#include "MaintenanceHaulersListView.h"
#include "MaintenanceRepairersListView.h"
#include "PartsView.h"

#include "actions/ActionsModel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/Roles.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MaintenanceFunctions.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "ENT/ENT_Tr.h"
#include "gaming/LogisticLinks.h"
#include "gaming/LogisticsConsign_ABC.h"
#include "gaming/LogMaintenanceConsign.h"
#include "protocol/Protocol.h"

namespace
{
    QAbstractButton* AddRadioButton( const QString& objectName,
                                     const QString& title,
                                     QObject* parent )
    {
        QRadioButton* button = new gui::RichWidget< QRadioButton >( objectName, 0 );
        button->setText( title );
        QObject::connect( button, SIGNAL( clicked() ), parent, SLOT( OnRadioButtonChanged() ) );
        return button;
    }

    template< typename T >
    T* AddResourceListView( const QString& objectName,
                            kernel::Controllers& controllers,
                            QWidget* parent )
    {
        T* view = new T( parent, controllers, false );
        view->setObjectName( objectName );
        for( int i = 3; i < view->model()->columnCount(); ++i )
            view->setColumnHidden( i, true );
        QObject::connect( view->selectionModel(), SIGNAL( currentRowChanged( const QModelIndex&, const QModelIndex& ) ),
                          parent,                 SLOT( OnSelectionChanged( const QModelIndex&, const QModelIndex& ) ) );
        return view;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog constructor
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
LogisticMaintenanceSelectionDialog::LogisticMaintenanceSelectionDialog( const QString& objectName,
                                                                        QWidget* parent,
                                                                        kernel::Controllers& controllers,
                                                                        actions::ActionsModel& actionsModel )
    : LogisticSelectionDialog_ABC( objectName, parent )
    , controller_( controllers.controller_ )
    , actionsModel_( actionsModel )
    , id_( 0 )
    , lastContext_( 0 )
    , handler_( controllers )
    , status_( sword::LogMaintenanceHandlingUpdate::finished )
    , availability_( 0 )
    , componentType_( 0 )
{
    setMinimumSize( 400, 400 );

    // Radio buttons
    automaticButton_ = AddRadioButton( "automated_selection_button_automatic", tr( "Resolve automatically" ), this );
    evacuateButton_ = AddRadioButton( "automated_selection_button_evacuate", tr( "Request evacuation by logistic superior" ), this );
    manualButton_ = AddRadioButton( "automated_selection_button_manual", "", this );

    // Content
    transporters_ = AddResourceListView< MaintenanceHaulersListView >( "manual_selection_transporters_listview", controllers, this );
    transporters_->SetFilter( [&] ( const kernel::Availability& availability )
    {
        return componentType_ &&
               availability.type_ &&
               availability.type_->GetMaintenanceFunctions() &&
               availability.type_->GetMaintenanceFunctions()->CanHaul( *componentType_ );
    } );
    diagnosers_ = AddResourceListView< MaintenanceRepairersListView >( "manual_selection_diagnosis_team_listview", controllers, this );

    auto* repair = new QWidget();
    auto* layout = new QVBoxLayout( repair );
    repairers_ = AddResourceListView< MaintenanceRepairersListView >( "manual_selection_repair_team_listview", controllers, this );
    parts_ = new PartsView( controllers, this );
    connect( parts_, SIGNAL( Updated() ), this, SLOT( UpdateDisplay() ) );
    layout->addWidget( repairers_ );
    layout->addWidget( parts_ );

    // Buttons
    QPushButton* cancelButton = new gui::RichPushButton( "automated_selection_button_cancel", tr( "Cancel" ) );
    acceptButton_ = new gui::RichPushButton( "automated_selection_button_accept", tr( "Ok" ) );
    acceptButton_->setDefault( true );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( reject() ) );
    connect( acceptButton_, SIGNAL( clicked() ), SLOT( accept() ) );

    // Layouts
    stack_ = new QStackedWidget();
    AddWidget( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection, transporters_ );
    AddWidget( sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection, diagnosers_ );
    AddWidget( sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection, repair );

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch( 1 );
    bottomLayout->addWidget( cancelButton );
    bottomLayout->addWidget( acceptButton_ );

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget( automaticButton_ );
    mainLayout->addWidget( evacuateButton_ );
    mainLayout->addWidget( manualButton_ );
    mainLayout->addWidget( stack_, 1 );
    mainLayout->addLayout( bottomLayout );
    setLayout( mainLayout );

    actionsModel_.RegisterHandler( [&]( const sword::SimToClient& message )
    {
        if( message.message().has_magic_action_ack() && lastContext_ == message.context() &&
            message.message().magic_action_ack().error_code() == sword::MagicActionAck_ErrorCode_error_invalid_parameter )
             QMessageBox::warning( this, tr( "SWORD" ), tr( "This request cannot be resolved." ) );
    } );
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog destructor
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
LogisticMaintenanceSelectionDialog::~LogisticMaintenanceSelectionDialog()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::AddWidget
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::AddWidget( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status, QWidget* widget )
{
    if( !stack_ )
        return;
    if( indexMap_.find( status ) != indexMap_.end() )
        throw MASA_EXCEPTION( "Widget already defined for status: " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status ) );
    indexMap_[ status ] = stack_->addWidget( widget );
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::SetCurrentWidget
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::SetCurrentWidget()
{
    if( !stack_ )
        return;
    auto it = indexMap_.find( status_ );
    if(  it == indexMap_.end() ||
         stack_->widget( it->second ) == 0 ||
         !handler_ )
    {
        reject();
        throw MASA_EXCEPTION( "No widget defined for status: " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) );
    }
    stack_->setCurrentIndex( it->second );
}

namespace
{
    bool CanRequestEvacuationBySuperior( const kernel::Entity_ABC* entity )
    {
        if( auto base = logistic_helpers::GetLogisticBase( entity ) )
            if( auto links = base->Retrieve< LogisticLinks >() )
                return links->GetCurrentSuperior() != 0;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::Show
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::Show( const LogisticsConsign_ABC& consign )
{
    if( consign.GetType() != eMaintenance )
        throw MASA_EXCEPTION( "Not supposed to display a maintenance dialog on a non-maintenance consign" );
    const LogMaintenanceConsign& maintenanceConsign = static_cast< const LogMaintenanceConsign& >( consign );
    id_ = consign.GetId();
    handler_ = consign.GetHandler();
    componentType_ = maintenanceConsign.GetEquipment();
    status_ = maintenanceConsign.GetStatus();
    SetCurrentWidget();
    setWindowTitle( tr( "Request #%1 - %2" ).arg( id_ ).arg( QString::fromStdString( ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) ) ) );
    manualButton_->setChecked( true );
    evacuateButton_->setEnabled( CanRequestEvacuationBySuperior( handler_ ) );
    if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
    {
        manualButton_->setText( tr( "Select tow truck" ) );
        transporters_->selectionModel()->clear();
        transporters_->SelectEntity( handler_ );
    }
    else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
    {
        manualButton_->setText( tr( "Select diagnosis team" ) );
        diagnosers_->selectionModel()->clear();
        diagnosers_->SelectEntity( handler_ );
    }
    else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
    {
        manualButton_->setText( tr( "Select repair team" ) );
        repairers_->selectionModel()->clear();
        repairers_->SelectEntity( handler_ );
        parts_->Select( consign.GetHandler(), maintenanceConsign );
    }
    UpdateDisplay();
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::accept
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::accept()
{
    if( automaticButton_->isChecked() )
        actionsModel_.PublishSelectNewLogisticState( id_ );
    else if( manualButton_->isChecked() )
    {
        if( !availability_ || !availability_->type_ )
            throw MASA_EXCEPTION( "Not supposed to accept in manual without an availability or its equipment type" );
        if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
            lastContext_ = actionsModel_.PublishSelectMaintenanceTransporter( id_, availability_->type_->GetId() );
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
            lastContext_ = actionsModel_.PublishSelectMaintenanceDiagnosisTeam( id_, availability_->type_->GetId() );
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
            lastContext_ = actionsModel_.PublishSelectMaintenanceRepairTeam( id_, availability_->type_->GetId() );
        else
            throw MASA_EXCEPTION( "Unhandled status " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) );
    }
    else if( evacuateButton_->isChecked() )
        actionsModel_.PublishTransferToLogisticSuperior( id_ );
    Purge();
    LogisticSelectionDialog_ABC::accept();
    // Wait for ack here, if error do not accept and display it
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::Purge
// Created: ABR 2014-01-31
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::Purge()
{
    id_ = 0;
    handler_ = 0;
    availability_ = 0;
    status_ = sword::LogMaintenanceHandlingUpdate::finished;
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::UpdateDisplay
// Created: ABR 2014-01-28
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::UpdateDisplay()
{
    bool manual = manualButton_->isChecked() && availability_ && availability_->available_ > 0;
    if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        manual &= parts_->IsValid();
    acceptButton_->setEnabled( automaticButton_->isChecked() || manual ||
                               evacuateButton_->isChecked() );
    stack_->setEnabled( manualButton_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::OnRadioButtonChanged
// Created: ABR 2014-01-28
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::OnRadioButtonChanged()
{
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::OnSelectionChanged
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::OnSelectionChanged( const QModelIndex& current, const QModelIndex& )
{
    availability_ = 0;
    if( current.isValid() && manualButton_->isChecked() )
    {
        if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
            availability_ = transporters_->model()->data( current, gui::Roles::DataRole ).value< const kernel::Availability* >();
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
            availability_ = diagnosers_->model()->data( current, gui::Roles::DataRole ).value< const kernel::Availability* >();
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
            availability_ = repairers_->model()->data( current, gui::Roles::DataRole ).value< const kernel::Availability* >();
        else
            throw MASA_EXCEPTION( "Unhandled status " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) );
    }
    UpdateDisplay();
}

namespace
{
    template< typename T >
    void Select( const T* source, const std::function< void( const QModelIndex& ) >& select )
    {
        const auto indexes = source->selectionModel()->selectedIndexes();
        select( indexes.isEmpty() ? QModelIndex() : indexes.front() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::NotifyUpdated
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::NotifyUpdated( const kernel::MaintenanceStates_ABC& state )
{
    if( !isVisible() ||
        !handler_ ||
        handler_->Retrieve< kernel::MaintenanceStates_ABC >() != &state )
        return;
    if( manualButton_->isChecked() )
    {
        auto selector = [&]( const QModelIndex& current ){ OnSelectionChanged( current, QModelIndex() ); };
        if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
            Select( transporters_, selector );
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
            Select( diagnosers_, selector );
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
            Select( repairers_, selector );
        else
            throw MASA_EXCEPTION( "Unhandled status " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) );
    }
}

void LogisticMaintenanceSelectionDialog::NotifyUpdated( const kernel::Profile_ABC& profile )
{
    automaticButton_->setVisible( profile.IsSupervision() );
}
