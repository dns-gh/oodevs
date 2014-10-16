// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "InhabitantCreationPanel.h"
#include "moc_InhabitantCreationPanel.cpp"

#include "clients_gui/LocationCreator.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichLabel.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichSpinBox.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/SimpleLocationDrawer.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "preparation/AgentsModel.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantCreationPanel::InhabitantCreationPanel( QWidget* parent,
                                                  gui::PanelStack_ABC& panel,
                                                  kernel::Controllers& controllers,
                                                  const AgentTypes& types,
                                                  AgentsModel& agentsModel,
                                                  const std::shared_ptr< gui::ParametersLayer >& layer,
                                                  const gui::GLView_ABC& tools )
    : InfoPanel_ABC( parent, panel, tr( "Populations" ), "InhabitantCreationPanel" )
    , controllers_( controllers )
    , tools_      ( tools )
    , resolver_   ( types )
    , agentsModel_( agentsModel )
    , location_   ( 0 )
{
    gui::SubObjectName subObject( "InhabitantCreationPanel" );

    QLabel* nameLabel = new QLabel( tr( "Name:" ) );
    name_ = new gui::RichLineEdit( "name" );

    QLabel* sideLabel = new QLabel( tr( "Side:" ) );
    teams_ = new gui::ValuedComboBox< const Team_ABC* >( "teams" );

    QLabel* typeLabel = new QLabel( tr( "Type:" ) );
    inhabitantTypes_ = new gui::ValuedComboBox< const InhabitantType* >( "inhabitantTypes" );
    inhabitantTypes_->SetSorting( true );
    connect( inhabitantTypes_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged() ) );

    QLabel* numberLabel = new QLabel( tr( "Number:" ) );
    number_ = new gui::RichSpinBox( "number", 0, 1 );
    number_->setValue( 1000 );

    position_ = new gui::RichLabel( "position", tr( "Location:" ) );
    position_->setMaximumHeight( 20 );
    locationCreator_ = new gui::LocationCreator( position_, tr( "New Population" ), layer, *this );

    locationLabel_ = new QLabel( tr( "---" ) );
    locationLabel_->setMinimumWidth( 100 );
    locationLabel_->setMaximumHeight( 50 );
    locationLabel_->setAlignment( Qt::AlignCenter );
    locationLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    gui::RichGroupBox* informationGroupBox = new gui::RichGroupBox( "Information", tr( "Information" ), this );
    QGridLayout* informationBoxLayout = new QGridLayout( informationGroupBox );
    informationBoxLayout->addWidget( nameLabel , 0, 0 );
    informationBoxLayout->addWidget( name_ , 0, 1 );
    informationBoxLayout->addWidget( sideLabel , 1, 0 );
    informationBoxLayout->addWidget( teams_ , 1, 1 );
    informationBoxLayout->addWidget( typeLabel , 2, 0 );
    informationBoxLayout->addWidget( inhabitantTypes_ , 2, 1 );
    informationBoxLayout->addWidget( numberLabel , 3, 0 );
    informationBoxLayout->addWidget( number_ , 3, 1 );
    informationBoxLayout->addWidget( position_ , 4, 0 );
    informationBoxLayout->addWidget( locationLabel_ , 4, 1 );
    informationBoxLayout->setAlignment( Qt::AlignTop );
    informationBoxLayout->setMargin( 5 );

    gui::RichPushButton* ok = new gui::RichPushButton( "create", tr( "Create" ), this );
    connect( ok, SIGNAL( clicked() ), this, SLOT( Commit2() ) );

    QWidget* box = new QWidget( this );
    QBoxLayout* layout = new QBoxLayout( box, QBoxLayout::TopToBottom, 0, 5 );
    layout->addWidget( informationGroupBox );
    layout->addWidget( ok );
    layout->addStretch( 1 );
    layout->setAlignment( Qt::AlignTop );
    setWidget( box );

    controllers_.Register( *this );

}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel destructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantCreationPanel::~InhabitantCreationPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::Draw
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::Draw( gui::Viewport_ABC& /*viewport*/ )
{
    if( location_ )
    {
        gui::SimpleLocationDrawer visitor( tools_ );
        location_->Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::Handle
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::Handle( Location_ABC& location )
{
    delete location_;
    location_ = 0;
    location_ = &location;
    if( location.IsValid() )
        locationLabel_->setText( location.GetName() );
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::Commit
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::Commit2()
{
    if( CheckValidity() )
    {
        agentsModel_.CreateInhabitant( const_cast< Team_ABC& >( *teams_->GetValue() ), *inhabitantTypes_->GetValue(), number_->value(), name_->text(), *location_ );
        name_->setText( "" );
        ResetLocation();
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::showEvent
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::showEvent( QShowEvent* )
{
    FillObjectTypes();
    OnTypeChanged();
    controllers_.Register( *locationCreator_ );
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::hideEvent
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::hideEvent( QHideEvent* )
{
    if( locationCreator_ )
        controllers_.Unregister( *locationCreator_ );
    ResetLocation();
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::FillObjectTypes
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::FillObjectTypes()
{
    inhabitantTypes_->Clear();
    tools::Iterator< const InhabitantType& > it( resolver_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        const InhabitantType& element = it.NextElement();
        inhabitantTypes_->AddItem( element.GetName(), &element );
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::CheckValidity
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
bool InhabitantCreationPanel::CheckValidity() const
{
    if( !teams_->count() || !teams_->GetValue() )
        return false;
    if( !inhabitantTypes_->count() || !inhabitantTypes_->GetValue() )
        return false;
    if( ! location_ )
    {
        position_->Warn( 3000 );
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::NotifyUpdated
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    FillObjectTypes();
    OnTypeChanged();
    Show();
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::NotifyUpdated
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    inhabitantTypes_->Clear();
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::NotifyCreated
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::NotifyCreated( const Team_ABC& team )
{
    if( teams_->GetItemIndex( &team ) != -1 )
        return;
    teams_->AddItem( team.GetName(), &team );
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::NotifyUpdated
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::NotifyUpdated( const kernel::Team_ABC& team )
{
    const int index = teams_->GetItemIndex( &team );
    if( index != -1 )
        teams_->changeItem( team.GetName(), index );
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::NotifyDeleted
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::NotifyDeleted( const Team_ABC& team )
{
    teams_->RemoveItem( &team );
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::OnTypeChanged
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::OnTypeChanged()
{
    if( inhabitantTypes_->Count() != 0 )
    {
        const InhabitantType* type = inhabitantTypes_->GetValue();
        if( !type )
            return;
        locationCreator_->Allow( false, false, true, false, false );
        if( location_ && !location_->IsValid() )
            ResetLocation();
    }
}

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel::ResetLocation
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
void InhabitantCreationPanel::ResetLocation()
{
    delete location_; location_ = 0;
    locationLabel_->setText( tr( "---" ) );
}