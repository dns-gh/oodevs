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
#include "DiagnosisUnitView.h"
#include "MaintenanceHaulersListView.h"
#include "MaintenanceRepairersListView.h"
#include "PartsView.h"
#include "actions/ActionsModel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/Roles.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/BreakdownType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MaintenanceFunctions.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ENT/ENT_Tr.h"
#include "gaming/LogisticLinks.h"
#include "gaming/LogisticsConsign_ABC.h"
#include "gaming/LogMaintenanceConsign.h"
#include "protocol/Protocol.h"

namespace
{
    const int acknowledgeTimeout = 10000;

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
        for( int i = 4; i < view->model()->columnCount(); ++i )
            view->setColumnHidden( i, true );
        QObject::connect( view->selectionModel(), SIGNAL( currentRowChanged( const QModelIndex&, const QModelIndex& ) ),
                          parent,                 SLOT( OnSelectionChanged( const QModelIndex&, const QModelIndex& ) ) );
        return view;
    }

    QString GetErrorText( const sword::MagicActionAck& ack )
    {
        if( ack.has_result() && ack.result().elem_size() > 0 )
        {
            const auto& code = ack.result().elem( 0 ).value();
            if( code.size() > 0 && code.Get( 0 ).has_identifier() )
            {
                switch( code.Get( 0 ).identifier() )
                {
                case sword::consign_already_resolved:
                    return tools::translate( "LogisticMaintenanceSelectionDialog", "The request is already resolved." );
                case sword::diagnosis_team_unavailable:
                    return tools::translate( "LogisticMaintenanceSelectionDialog", "The diagnosis team is unavailable." );
                case sword::repair_team_unavailable:
                    return tools::translate( "LogisticMaintenanceSelectionDialog", "The repair team is unavailable." );
                case sword::transporter_unavailable:
                    return tools::translate( "LogisticMaintenanceSelectionDialog", "The tow truck is unavailable." );
                }
            }
        }
        return tools::translate( "LogisticMaintenanceSelectionDialog", "This request cannot be resolved." );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog constructor
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
LogisticMaintenanceSelectionDialog::LogisticMaintenanceSelectionDialog( const QString& objectName,
                                                                        QWidget* parent,
                                                                        kernel::Controllers& controllers,
                                                                        actions::ActionsModel& actionsModel,
                                                                        gui::DisplayExtractor& extractor )
    : LogisticSelectionDialog_ABC( objectName, parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , id_( 0 )
    , lastContext_( 0 )
    , handler_( controllers )
    , consumer_( controllers )
    , status_( sword::LogMaintenanceHandlingUpdate::finished )
    , componentType_( 0 )
    , breakdownType_( 0 )
    , availability_( 0 )
{
    resize( 500, 400 );

    // Radio buttons
    automaticButton_ = AddRadioButton( "automated_selection_button_automatic", tr( "Resolve automatically" ), this );
    evacuateButton_ = AddRadioButton( "automated_selection_button_evacuate", tr( "Request evacuation by logistic superior" ), this );
    manualButton_ = AddRadioButton( "automated_selection_button_manual", "", this );

    // Content
    auto* transport = new QWidget();
    auto* transportLayout = new QVBoxLayout( transport );
    auto transportersWidget = new QWidget( this );
    auto transportersLayout = new QVBoxLayout( transportersWidget );
    transportersLayout->setMargin( 0 );
    transporters_ = AddResourceListView< MaintenanceHaulersListView >( "manual_selection_transporters_listview", controllers, this );
    transporters_->SetFilter( [&] ( const kernel::Availability& availability )
    {
        return componentType_ &&
               availability.type_ &&
               availability.type_->GetMaintenanceFunctions() &&
               availability.type_->GetMaintenanceFunctions()->CanHaul( *componentType_ );
    } );
    transporters_->setColumnHidden( 7, false ); // tow truck capacity column
    equipmentWeight_ = new QLabel( this );
    equipmentWeight_->setVisible( false );
    transportersLayout->addWidget( transporters_ );
    transportersLayout->addWidget( equipmentWeight_ );
    destinationBox_ = new gui::RichGroupBox( "manual_selection_transporters_groupbox", tr( "Select diagnosis / repair unit" ) );
    auto* destinationLayout = new QVBoxLayout( destinationBox_ );
    destinations_ = new DiagnosisUnitView( this, extractor );
    connect( destinations_, SIGNAL( DestinationSelected( unsigned int ) ), SLOT( OnDestinationSelected( unsigned int ) ) );
    destinationLayout->addWidget( destinations_ );
    destinationBox_->setCheckable( true );
    destinationBox_->setChecked( false );
    destinationBox_->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    QSplitter* split = new QSplitter();
    split->setOrientation( Qt::Vertical );
    split->setChildrenCollapsible( false );
    split->addWidget( transportersWidget );
    split->addWidget( destinationBox_ );
    transportLayout->addWidget( split );

    diagnosers_ = AddResourceListView< MaintenanceRepairersListView >( "manual_selection_diagnosis_team_listview", controllers, this );

    auto* repair = new QWidget();
    auto* repairLayout = new QVBoxLayout( repair );
    repairers_ = AddResourceListView< MaintenanceRepairersListView >( "manual_selection_repair_team_listview", controllers, this );
    repairers_->SetFilter( [&] ( const kernel::Availability& availability )
    {
        return breakdownType_ &&
               availability.type_ &&
               availability.type_->GetMaintenanceFunctions() &&
               availability.type_->GetMaintenanceFunctions()->CanRepair( *breakdownType_ );
    } );
    duration_ = new QLabel( this );
    duration_->setVisible( false );
    parts_ = new PartsView( controllers, this );
    connect( parts_, SIGNAL( Updated() ), this, SLOT( UpdateDisplay() ) );
    repairLayout->addWidget( repairers_ );
    repairLayout->addWidget( duration_ );
    repairLayout->addWidget( parts_ );

    // Buttons
    QPushButton* cancelButton = new gui::RichPushButton( "automated_selection_button_cancel", tr( "Cancel" ) );
    acceptButton_ = new gui::RichPushButton( "automated_selection_button_accept", tr( "Ok" ) );
    acceptButton_->setDefault( true );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( reject() ) );
    connect( acceptButton_, SIGNAL( clicked() ), SLOT( OnOkClicked() ) );

    // Layouts
    stack_ = new QStackedWidget();
    AddWidget( sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection, transport );
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

    timeout_.setInterval( acknowledgeTimeout );
    timeout_.setSingleShot( true );
    connect( &timeout_, SIGNAL( timeout() ), this, SLOT( OnTimeout() ) );

    actionsModel_.RegisterHandler( [&]( const sword::SimToClient& message )
    {
        if( lastContext_ == 0 || !isVisible() ||
            !message.message().has_magic_action_ack() || lastContext_ != message.context() )
            return;
        Purge();
        if( message.message().magic_action_ack().error_code() == sword::MagicActionAck_ErrorCode_error_invalid_parameter )
        {
            QMessageBox::warning( this, tr( "SWORD" ), GetErrorText( message.message().magic_action_ack() ) );
            reject();
        }
        else
            accept();
    } );
    connect( destinationBox_, SIGNAL( toggled( bool ) ), SLOT( OnDestinationToggled( bool ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog destructor
// Created: ABR 2014-01-23
// -----------------------------------------------------------------------------
LogisticMaintenanceSelectionDialog::~LogisticMaintenanceSelectionDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::OnTimeout
// Created: ABR 2014-02-19
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::OnTimeout()
{
    lastContext_ = 0;
    QMessageBox::warning( this, tr( "SWORD" ), tr( "No response received from the simulation." ) );
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

namespace
{
    bool CanRequestEvacuationBySuperior( const kernel::Entity_ABC& entity )
    {
        if( auto base = logistic_helpers::GetLogisticBase( &entity ) )
            if( auto links = base->Retrieve< LogisticLinks >() )
                return links->GetCurrentSuperior() != 0;
        return false;
    }
    template< typename T >
    void UpdateView( T* view, const kernel::Entity_ABC& entity, QAbstractButton* button, const QString& buttonText )
    {
        button->setText( buttonText );
        view->selectionModel()->reset();
        view->clearFocus();
        view->SelectEntity( &entity );
    }
}

namespace
{
    void GetDestinations( const kernel::Entity_ABC& handler, std::vector< const kernel::Entity_ABC* >& destinations )
    {
        if( auto tactical = handler.Retrieve< kernel::TacticalHierarchies >() )
        {
            auto it = tactical->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& child = it.NextElement();
                const kernel::MaintenanceStates_ABC* state = child.Retrieve< kernel::MaintenanceStates_ABC >();
                if( child.GetTypeName() == kernel::Agent_ABC::typeName_ && state && !state->GetDispoRepairers().empty() )
                    destinations.push_back( &child );
                else
                    GetDestinations( child, destinations );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::SetCurrentStatus
// Created: ABR 2014-01-29
// -----------------------------------------------------------------------------
bool LogisticMaintenanceSelectionDialog::SetCurrentStatus( sword::LogMaintenanceHandlingUpdate_EnumLogMaintenanceHandlingStatus status )
{
    auto it = indexMap_.find( status );
    if( !stack_ ||
        it == indexMap_.end() ||
        stack_->widget( it->second ) == 0 ||
        !handler_ ||
        !componentType_ ||
        !breakdownType_ )
        return false;
    status_ = status;
    stack_->setCurrentIndex( it->second );
    setWindowTitle( tr( "Request #%1 - %2" ).arg( id_ ).arg( QString::fromStdString( ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) ) ) );
    if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
    {
        UpdateView( transporters_, *handler_, manualButton_, tr( "Select tow truck" ) );
        std::vector< const kernel::Entity_ABC* > destinations;
        GetDestinations( *handler_, destinations );
        destinations_->Fill( destinations, *consumer_, 0 );
        destinationBox_->setVisible( true );
        equipmentWeight_->setText( tr( "Vehicle weight: %1 (t)" ).arg( componentType_->GetWeight() ) );
        equipmentWeight_->setVisible( true );
    }
    else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
        UpdateView( diagnosers_, *handler_, manualButton_, tr( "Select diagnosis team" ) );
    else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
    {
        UpdateView( repairers_, *handler_, manualButton_, tr( "Select repair team" ) );
        duration_->setText( tr( "Estimated repair duration: %1" ).arg( tools::DurationFromSeconds( breakdownType_->GetRepairTime() ) ) );
        duration_->setVisible( true );
    }
    parts_->Fill( breakdownType_->GetParts() );
    manualButton_->setChecked( true );
    evacuateButton_->setEnabled( CanRequestEvacuationBySuperior( *handler_ ) );
    UpdateDisplay();
    return true;
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::Show
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::Show( const LogisticsConsign_ABC& consign )
{
    Purge();
    if( consign.GetType() != eMaintenance )
        throw MASA_EXCEPTION( "Not supposed to display a maintenance dialog on a non-maintenance consign" );
    const LogMaintenanceConsign& maintenanceConsign = static_cast< const LogMaintenanceConsign& >( consign );
    id_ = consign.GetId();
    handler_ = consign.GetHandler();
    consumer_ = consign.GetConsumer();
    componentType_ = maintenanceConsign.GetEquipment();
    breakdownType_ = maintenanceConsign.GetBreakdown();
    if( !SetCurrentStatus( maintenanceConsign.GetStatus() ) )
        return;
    show();
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::OnOkClicked
// Created: ABR 2014-01-27
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::OnOkClicked()
{
    if( automaticButton_->isChecked() )
        lastContext_ = actionsModel_.PublishSelectNewLogisticState( id_ );
    else if( manualButton_->isChecked() )
    {
        if( !availability_ || !availability_->type_ )
            throw MASA_EXCEPTION( "Not supposed to accept in manual without an availability or its equipment type" );
        if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
            lastContext_ = actionsModel_.PublishSelectMaintenanceTransporter( id_, availability_->type_->GetId(), selectedDestination_ );
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
            lastContext_ = actionsModel_.PublishSelectMaintenanceDiagnosisTeam( id_, availability_->type_->GetId() );
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
            lastContext_ = actionsModel_.PublishSelectMaintenanceRepairTeam( id_, availability_->type_->GetId() );
        else
            throw MASA_EXCEPTION( "Unhandled status " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) );
    }
    else if( evacuateButton_->isChecked() )
        lastContext_ = actionsModel_.PublishTransferToLogisticSuperior( id_ );
    timeout_.start();
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::Purge
// Created: ABR 2014-01-31
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::Purge()
{
    id_ = 0;
    handler_ = 0;
    componentType_ = 0;
    breakdownType_ = 0;
    availability_ = 0;
    lastContext_ = 0;
    selectedDestination_ = boost::none;
    status_ = sword::LogMaintenanceHandlingUpdate::finished;
    duration_->setVisible( false );
    destinationBox_->setVisible( false );
    destinationBox_->setChecked( false );
    destinations_->Purge();
    timeout_.stop();
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
    else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection )
        manual &= destinationBox_->isChecked() && selectedDestination_ || !destinationBox_->isChecked();
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
        if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_transporter_selection && consumer_ && handler_ )
        {
            availability_ = transporters_->model()->data( current, gui::Roles::DataRole ).value< const kernel::Availability* >();
            std::vector< const kernel::Entity_ABC* > destinations;
            GetDestinations( *handler_, destinations );
            destinations_->Fill( destinations, *consumer_, availability_->type_ );
        }
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_diagnosis_team_selection )
            availability_ = diagnosers_->model()->data( current, gui::Roles::DataRole ).value< const kernel::Availability* >();
        else if( status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        {
            availability_ = repairers_->model()->data( current, gui::Roles::DataRole ).value< const kernel::Availability* >();
            if( availability_ )
                parts_->SelectEntity( availability_->entity_ );
            else
                throw MASA_EXCEPTION( "Not supposed to select a line without availability in a ResourcesListView_ABC" );
        }
        else
            throw MASA_EXCEPTION( "Unhandled status " + ENT_Tr::ConvertFromLogMaintenanceHandlingStatus( status_ ) );
    }
    else if( !current.isValid() && status_ == sword::LogMaintenanceHandlingUpdate::waiting_for_repair_team_selection )
        parts_->SelectEntity( 0 );
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

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::OnDestinationSelected
// Created: SLI 2014-03-19
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::OnDestinationSelected( unsigned int destination )
{
    selectedDestination_ = destination;
    UpdateDisplay();
}

// -----------------------------------------------------------------------------
// Name: LogisticMaintenanceSelectionDialog::OnDestinationSelected
// Created: SLI 2014-03-19
// -----------------------------------------------------------------------------
void LogisticMaintenanceSelectionDialog::OnDestinationToggled( bool enabled )
{
    if( !enabled )
    {
        selectedDestination_ = boost::none;
        destinations_->reset();
    }
    UpdateDisplay();
}
