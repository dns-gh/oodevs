// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "MissionInterface_ABC.h"
#include "moc_MissionInterface_ABC.cpp"
#include "clients_gui/RichCheckBox.h"
#include "clients_gui/RichDateTimeEdit.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"
#include "clients_kernel/Time_ABC.h"
#include "ParamComboBox.h"
#include "tools/ExerciseConfig.h"

#include <QtWebKit/qwebview.h>

using namespace actions::gui;

namespace
{
    QWidget* CreateTab( QTabWidget* parent, const QString& title, bool enabled = true )
    {
        // Scroll
        QScrollArea* scrollArea = new QScrollArea( parent );
        scrollArea->setFrameStyle( QFrame::Box | QFrame::Sunken );
        scrollArea->setWidgetResizable( true );
        // Box
        QWidget* box = new QWidget( parent );
        QVBoxLayout* layout = new QVBoxLayout( box );
        // Tab
        Q3VBox* tab = new Q3VBox( parent );
        tab->setMargin( 5 );
        tab->setSpacing( 5 );
        // Parent
        scrollArea->setWidget( box );
        parent->addTab( scrollArea, title );
        parent->setTabEnabled( scrollArea, enabled );
        layout->addWidget( tab );
        layout->addStretch( 10 );
        return tab;
    }

    QWidget* CreateLineField( const QString& name, const QString& text, QWidget* parent )
    {
        QTextEdit* textEdit = new QTextEdit( text );
        textEdit->setReadOnly( true );
        QGroupBox* group = new QGroupBox( name, parent );
        QVBoxLayout* layout = new QVBoxLayout( group );
        layout->addWidget( textEdit );
        return group;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( QWidget* parent, const QString& name, kernel::Controllers& controllers,
                                            actions::ActionsModel& actionModel, const tools::ExerciseConfig& config )
    : QTabWidget( parent )
    , ParamInterface_ABC()
    , controllers_( controllers )
    , model_( actionModel )
    , config_( config )
    , entity_( controllers )
    , planned_( false )
{
    setObjectName( name );
    setMinimumSize( 280, 250 );
    mainTab_ = CreateTab( this, tr( "Mandatory" ) );
    optionalTab_ = CreateTab( this, tr( "Optional" ) );
    helpTab_ = CreateTab( this, tr( "Help" ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Purge
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Purge()
{
    entity_ = 0;
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
    {
        (*it)->RemoveFromController();
        delete *it;
    }
    parameters_.clear();

    // Some params store their widget, so widget deletion need to be after parameters
    for( auto it = widgetToDelete_.begin(); it != widgetToDelete_.end(); ++it )
        delete *it;
    widgetToDelete_.clear();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Fill
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Fill( InterfaceBuilder_ABC& builder, const kernel::Entity_ABC& entity, const kernel::OrderType& order, const std::string& missionSheetPhysicalTag )
{
    entity_ = &entity;
    order_ = &order;
    title_ = order.GetName().c_str();

    setVisible( false ); // $$$$ ABR 2013-06-04: For some reason the current tab display doesn't refresh, probably because of Q3VBox. This will be fix when Param_ABC::BuildInterface will take a QVBoxLayout as parent instead of a QWidget
    builder.BuildAll( *this, entity, order );
    setVisible( true );
    setCurrentIndex( 0 );

    // Help tab
    std::string doctrine = order.GetDoctrineInformation();
    std::string usage = order.GetUsageInformation();
    tools::Path fileName = config_.GetPhysicalChildPath( missionSheetPhysicalTag ) / tools::Path::FromUTF8( order.GetName() ) + ".html";
    if( fileName.IsRegularFile() || ( !doctrine.empty() && !usage.empty() ) )
    {
        if( fileName.IsRegularFile() )
        {
            fileName.MakePreferred();
            QWebView* missionSheetText = new QWebView( helpTab_ );
            missionSheetText->setContextMenuPolicy( Qt::NoContextMenu );
            missionSheetText->load( QUrl( fileName.Normalize().ToUTF8().c_str() ) );
            widgetToDelete_.push_back( missionSheetText );
        }
        else
        {
            if( !doctrine.empty() )
                widgetToDelete_.push_back( CreateLineField( tr( "Doctrine" ), doctrine.c_str(), helpTab_ ) );
            if( !usage.empty() )
                widgetToDelete_.push_back( CreateLineField( tr( "Usage" ), usage.c_str(), helpTab_ ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        b = (*it)->CheckValidity() && b;
    return b;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CommitTo
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CommitTo( actions::Action_ABC& action ) const
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->CommitTo( action );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::IsEmpty
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::IsEmpty() const
{
    return parameters_.empty();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Title
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
QString MissionInterface_ABC::Title() const
{
    return title_;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddParameter( const QString& objectName, Param_ABC& parameter )
{
    parameters_.push_back( &parameter );
    QWidget* widget = parameter.BuildInterface( objectName, parameter.IsOptional() ? optionalTab_ : mainTab_ );
    widgetToDelete_.push_back( widget );
    parameter.RegisterIn( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Draw( gui::GlTools_ABC& tools, ::gui::Viewport_ABC& extent ) const
{
    if( !entity_ )
        return;
    for( auto it = parameters_.begin() ; it != parameters_.end() ; ++it )
    {
        const geometry::Point2f p = entity_->Get< kernel::Positions >().GetPosition();
        extent.SetHotpoint( p );
        (*it)->Draw( p, extent, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::GetIndex
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
int MissionInterface_ABC::GetIndex( Param_ABC* param ) const
{
    for( int i = 0; i < (int)parameters_.size(); ++i )
        if( parameters_[ i ] == param )
            return i;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::SetPlanned
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface_ABC::SetPlanned( bool planned )
{
    planned_ = planned;
}
