// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MedicalCapacityUpdateDialog.h"
#include "moc_MedicalCapacityUpdateDialog.cpp"
#include "actions/ActionTiming.h"
#include "actions/ActionTasker.h"
#include "actions/ObjectMagicAction.h"
#include "actions/ParameterList.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/MedicalTreatmentType.h"
#include "gaming/MedicalTreatmentAttribute.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Capacity
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
MedicalCapacityUpdateDialog::Capacity::Capacity( QWidget* parent, const MedicalTreatmentCapacity& capacity )
    : name_ ( capacity.name_ )
    , id_ ( capacity.type_->GetId() )
{
    Q3HBox* box = new Q3HBox( parent );
    Q3HBox* lhs = new Q3HBox( box );
    new QLabel( QString( name_.c_str() ), lhs );

    Q3HBox* rhs = new Q3HBox( box );
    patients_ = new QSpinBox( 0, capacity.baseline_, 1, rhs );
    patients_->setValue( capacity.baseline_ - capacity.available_ );

    new QLabel( QString( " / " ), rhs );
    baseline_ = new QSpinBox( 0, std::max( (int)capacity.baseline_, 500 ), 1, rhs );
    baseline_->setValue( capacity.baseline_ );
    baseline_->setDisabled( true );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Capacity
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
MedicalCapacityUpdateDialog::Capacity::Capacity( const Capacity& rhs )
    : name_ ( rhs.name_ )
    , id_ ( rhs.id_ )
    , patients_ ( rhs.patients_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Capacity::Update
// Created: LDC 2010-07-07
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::Capacity::Update( const MedicalTreatmentCapacity& capacity )
{
    patients_->setMaxValue( capacity.baseline_ );
    patients_->setValue( capacity.baseline_ - capacity.available_ );
    baseline_->setValue( capacity.baseline_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog constructor
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
MedicalCapacityUpdateDialog::MedicalCapacityUpdateDialog( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation, Publisher_ABC& publisher, const kernel::Profile_ABC& profile )
    : QDialog( parent )
    , controllers_( controllers )
    , actionsModel_ ( actionsModel )
    , static_ ( staticModel )
    , simulation_ ( simulation )
    , publisher_( publisher )
    , profile_( profile )
    , selected_( controllers )
{
    CreateCanvas();
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::CreateCanvas
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::CreateCanvas()
{
    setCaption( tr( "Update medical capacity" ) );
    Q3VBoxLayout* layout = new Q3VBoxLayout( this );
    layout->setSpacing( 5 );
    layout->setMargin( 5 );
//
    // Equipment and personal

    // QVBox* vbox = new QVBox( this, tr( "MedicalTreatment Type:" ) );
    {
        Q3GroupBox* canvas = new Q3GroupBox( 1, Qt::Horizontal, tr( "MedicalTreatment Type" ), this );
        canvas->setFlat( true );
        new QLabel( tr( "Hospital ID:" ), canvas );
        referenceID_ = new QLineEdit( canvas );
        // referenceID->setText(  );
        referenceID_->setDisabled( true );
        layout->add( canvas );
    }
    {
        Q3GroupBox* canvas = new Q3GroupBox( 1, Qt::Horizontal, tr( "Status" ), this );
        new QLabel( tr( "Status:" ), canvas );
        status_ = new gui::ValuedComboBox< std::string >( canvas );
        status_->setDisabled( true );
        layout->add( canvas );
    }
    {
        Q3GroupBox* canvas = new Q3GroupBox( 1, Qt::Horizontal, tr( "Doctors" ), this );
        new QLabel( tr( "Doctors:" ), canvas );
        doctors_ = new QSpinBox( 0, 1000, 1, canvas );
        layout->add( canvas );
    }
    {
        Q3GroupBox* canvas = new Q3GroupBox( 1, Qt::Horizontal, tr( "Bed Capacities (Patients/Baseline)" ), this );
        vbox_ = new Q3VBox( canvas );
        // new QLabel( tr( "Bed Capacities:" ), vbox_ );
        // FillCapacityTypes( vbox_ );
        layout->add( canvas );
    }

    Q3HBox* box = new Q3HBox( this );
    layout->addWidget( box );
    QPushButton* okButton = new QPushButton( tr( "Ok" ), box );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ), box );
    okButton->setDefault( true );

    connect( okButton    , SIGNAL( clicked() ), SLOT( Validate() ) );
    connect( cancelButton, SIGNAL( clicked() ), SLOT( Reject() ) );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog destructor
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
MedicalCapacityUpdateDialog::~MedicalCapacityUpdateDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::NotifyContextMenu
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu )
{
    if( profile_.CanDoMagic( object ) && ( object.GetType().HasMedicalCapacity() || object.Retrieve< kernel::MedicalTreatmentAttribute_ABC >() ) )
    {
        selected_ = &object;
        Q3PopupMenu* subMenu = menu.SubMenu( "Order", tr( "Magic orders" ) );
        subMenu->insertItem( tr( "Update medical capacity" ), this, SLOT( Show() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Show
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::Show()
{
    if( !selected_ )
        return;
    InitializeCapacities();
    show();
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::InitializeCapacities
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::InitializeCapacities()
{
    typedef std::vector< MedicalTreatmentCapacity >::const_iterator CIT_MedicalCapacities;
    std::map< int, MedicalTreatmentCapacity > capacities;
    const MedicalTreatmentAttribute* att = static_cast< const MedicalTreatmentAttribute* >( selected_->Retrieve< kernel::MedicalTreatmentAttribute_ABC >() );
    if( att )
    {
        att->FillCapacities( capacities );
        referenceID_->setText( QString( att->GetReferenceId().c_str() ) );
        doctors_->setValue( att->GetDoctors() );
    }
    // clear vbox children
    if( capacities_.empty() )
    {
        for( std::map< int, MedicalTreatmentCapacity >::const_iterator it = capacities.begin(); it != capacities.end(); ++it )
            capacities_.push_back( new Capacity( vbox_, it->second ) );
    }
    else
    {
        boost::ptr_vector< Capacity >::iterator itCapa = capacities_.begin();
        for( std::map< int, MedicalTreatmentCapacity >::const_iterator it = capacities.begin(); it != capacities.end() && itCapa != capacities_.end(); ++it, ++itCapa )
            itCapa->Update( it->second );
    }

}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Validate
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::Validate()
{
    if( ! selected_ )
        return;

    accept();

    actions::parameters::ParameterList& list = *new actions::parameters::ParameterList( kernel::OrderParameter( "Medical Treatment", "list", false ) );
    FillAttributes( list );
    Send( list );
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Reject
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::Reject()
{
    reject();
    selected_ = 0;
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Send
// Created: JCR 2010-10-22
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::Send( actions::parameters::ParameterList& attribute ) const
{
    kernel::MagicActionType& actionType = static_cast< tools::Resolver< kernel::MagicActionType, std::string >& > ( static_.types_ ).Get( "update_object" );
    actions::ObjectMagicAction* action = new actions::ObjectMagicAction( selected_, actionType, controllers_.controller_, true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    actions::parameters::ParameterList* attributesList = new actions::parameters::ParameterList( it.NextElement() );

    action->AddParameter( *attributesList );
    attributesList->AddParameter( attribute );
    action->Attach( *new actions::ActionTiming( controllers_.controller_, simulation_ ) );
    action->Attach( *new actions::ActionTasker( selected_, false ) );
    action->RegisterAndPublish( actionsModel_ );
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::Commit
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::FillAttributes( actions::parameters::ParameterList& attribute ) const
{
    attribute.AddIdentifier( "AttributeId", sword::ObjectMagicAction_Attribute_medical_treatment );
    attribute.AddString( "ExternalReferenceId", referenceID_->text().ascii() );
    attribute.AddQuantity( "Doctors", doctors_->value() );
    attribute.AddIdentifier( "Status", 0 ); // JCR TODO : Check status available ?

    actions::parameters::ParameterList& capacities = attribute.AddList( "BedCapacities" );
    int i = 0;
    for( CIT_Capacities it = capacities_.begin(); it != capacities_.end(); ++it, ++i )
    {
        actions::parameters::ParameterList& capacity = capacities.AddList( "Capacity" );
        capacity.AddIdentifier( "TypeId", it->id_ );
        capacity.AddQuantity( "BaseLineCount", it->baseline_->value() );
        capacity.AddQuantity( "AvailableCount", it->baseline_->value() - it->patients_->value() );
        capacity.AddQuantity( "EmergencyCount", 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: MedicalCapacityUpdateDialog::closeEvent
// Created: JCR 2010-06-28
// -----------------------------------------------------------------------------
void MedicalCapacityUpdateDialog::closeEvent( QCloseEvent * /*e*/ )
{
    Reject();
}

