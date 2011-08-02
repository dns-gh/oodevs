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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/SimpleLocationDrawer.h"
#include "clients_gui/LocationCreator.h"
#include "clients_gui/RichLabel.h"
#include "preparation/AgentsModel.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InhabitantCreationPanel constructor
// Created: SLG 2010-11-25
// -----------------------------------------------------------------------------
InhabitantCreationPanel::InhabitantCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const AgentTypes& types, AgentsModel& agentsModel, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools )
    : InfoPanel_ABC( parent, panel, tr( "Populations" ), "InhabitantCreationPanel" )
    , controllers_( controllers )
    , tools_      ( tools )
    , resolver_   ( types )
    , agentsModel_( agentsModel )
    , location_   ( 0 )
{
    Q3VBox* gBox = new Q3VBox( this );
    Q3GroupBox* groupBox = new Q3GroupBox( 2, Qt::Horizontal, tr( "Information" ), gBox );

    new QLabel( tr( "Name:" ), groupBox );
    name_ = new QLineEdit( groupBox );

    new QLabel( tr( "Side:" ), groupBox );
    teams_ = new gui::ValuedComboBox< const Team_ABC* >( groupBox );

    new QLabel( tr( "Type:" ), groupBox );
    inhabitantTypes_ = new gui::ValuedComboBox< const InhabitantType* >( groupBox );
    inhabitantTypes_->setSorting( true );

    new QLabel( tr( "Number:" ), groupBox );
    number_ = new QLineEdit( QString::number( 1000 ), groupBox );
    number_->setValidator( new QIntValidator( 1, 1000000000, number_ ) );

    position_ = new gui::RichLabel( tr( "Location:" ), groupBox );
    locationLabel_ = new QLabel( tr( "---" ), groupBox );
    locationLabel_->setMinimumWidth( 100 );
    locationLabel_->setAlignment( Qt::AlignCenter );
    locationLabel_->setFrameStyle( QFrame::Box | QFrame::Sunken );

    locationCreator_ = new gui::LocationCreator( position_, tr( "New Population" ), layer, *this );

    connect( inhabitantTypes_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged() ) );

    QPushButton* ok = new QPushButton( tr( "Create" ), gBox );
    connect( ok, SIGNAL( clicked() ), this, SLOT( Commit2() ) );
    setWidget( gBox );
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
void InhabitantCreationPanel::Draw( kernel::Viewport_ABC& /*viewport*/ )
{
    SimpleLocationDrawer visitor( tools_ );
    if( location_ )
        location_->Accept( visitor );
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
        agentsModel_.CreateInhabitant( const_cast< Team_ABC& >( *teams_->GetValue() ), *inhabitantTypes_->GetValue(), number_->text().toInt(), name_->text(), *location_ );
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