// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "MissionInterface.h"
#include "moc_MissionInterface.cpp"
#include "actions/Action_ABC.h"
#include "actions/Parameter_ABC.h"
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "actions_gui/Param_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Controllers.h"
#include "tools/ExerciseConfig.h"
#include <boost/assign.hpp>

#pragma warning( push, 0 )
#include <QtWebKit/qwebview.h>
#pragma warning( pop )

using namespace actions::gui;

std::map< E_MissionType, const std::string > MissionInterface::missionSheetPhysicalTags_ = boost::assign::map_list_of( eMissionType_Pawn, "units-mission-sheets-directory" )
                                                                                                                     ( eMissionType_Automat, "automata-mission-sheets-directory" )
                                                                                                                     ( eMissionType_Population, "crowds-mission-sheets-directory" )
                                                                                                                     ( eMissionType_FragOrder, "fragorders-mission-sheets-directory" );

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
        QWidget* tab = new QWidget;
        tab->setLayout( new QVBoxLayout );
        tab->layout()->setAlignment( Qt::AlignTop );
        tab->layout()->setMargin( 5 );
        tab->layout()->setSpacing( 5 );
        // Parent
        scrollArea->setWidget( box );
        parent->addTab( scrollArea, title );
        parent->setTabEnabled( scrollArea, enabled );
        layout->addWidget( tab );
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
// Name: MissionInterface constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface::MissionInterface( QWidget* parent, const QString& name, kernel::Controllers& controllers, const tools::ExerciseConfig& config )
    : QTabWidget( parent )
    , config_( config )
    , entity_( controllers )
    , order_( 0 )
    , type_( eNbrMissionType )
    , filling_( false )
{
    setObjectName( name );
    mainTab_ = CreateTab( this, tr( "Mandatory" ) );
    optionalTab_ = CreateTab( this, tr( "Optional" ) );
    helpTab_ = CreateTab( this, tr( "Help" ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface::~MissionInterface()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::PurgeParameters
// Created: ABR 2013-10-30
// -----------------------------------------------------------------------------
void MissionInterface::PurgeParameters()
{
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
// Name: MissionInterface::Purge
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface::Purge()
{
    entity_ = 0;
    order_ = 0;
    type_ = eNbrMissionType;
    PurgeParameters();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::Rebuild
// Created: ABR 2013-10-30
// -----------------------------------------------------------------------------
void MissionInterface::Rebuild( InterfaceBuilder_ABC& builder )
{
    PurgeParameters();
    if( order_ )
        Build( builder, *order_, type_ );
    SetEntity( entity_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::SetEntity
// Created: ABR 2013-06-11
// -----------------------------------------------------------------------------
void MissionInterface::SetEntity( const kernel::Entity_ABC* entity )
{
    entity_ = entity;
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->SetEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::Build
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface::Build( InterfaceBuilder_ABC& builder, const kernel::OrderType& order, E_MissionType type )
{
    type_ = type;
    order_ = &order;

    setVisible( false ); // $$$$ ABR 2013-06-04: For some reason the current tab display doesn't refresh, probably because of Q3VBox. This will be fix when Param_ABC::BuildInterface will take a QVBoxLayout as parent instead of a QWidget
    builder.BuildAll( *this, order );
    setVisible( true );
    setCurrentIndex( 0 );

    // Help tab
    std::string doctrine = order.GetDoctrineInformation();
    std::string usage = order.GetUsageInformation();
    tools::Path fileName = config_.GetPhysicalChildPath( missionSheetPhysicalTags_[ type ] ) / tools::Path::FromUTF8( order.GetName() ) + ".html";
    if( fileName.IsRegularFile() || ( !doctrine.empty() && !usage.empty() ) )
    {
        if( fileName.IsRegularFile() )
        {
            fileName.MakePreferred();
            QWebView* missionSheetText = new QWebView();
            helpTab_->layout()->addWidget( missionSheetText );
            missionSheetText->setZoomFactor( 0.75 );
            missionSheetText->setContextMenuPolicy( Qt::NoContextMenu );
            missionSheetText->load( QUrl::fromLocalFile( QString::fromStdWString( fileName.Normalize().ToUnicode() ) ) );
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
// Name: MissionInterface::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface::CheckValidity()
{
    bool b = true;
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        b = (*it)->CheckValidity() && b;
    return b;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::CommitTo
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
void MissionInterface::CommitTo( actions::Action_ABC& action ) const
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->CommitTo( action );
}

namespace
{
    const actions::Parameter_ABC* FindParameter( const actions::Action_ABC& action, const QString& name )
    {
        for( auto it = action.CreateIterator(); it.HasMoreElements(); )
        {
            const actions::Parameter_ABC& parameter = it.NextElement();
            if( parameter.GetName() == name )
                return &parameter;
        }
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::FillFrom
// Created: ABR 2013-06-12
// -----------------------------------------------------------------------------
void MissionInterface::FillFrom( const actions::Action_ABC& action )
{
    filling_ = true;
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        if( auto param = FindParameter( action, ( *it )->GetName() ) )
            param->Accept( **it );
    filling_ = false;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::IsEmpty
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool MissionInterface::IsEmpty() const
{
    return parameters_.empty();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::Title
// Created: ABR 2013-06-04
// -----------------------------------------------------------------------------
QString MissionInterface::Title() const
{
    return order_ ? QString::fromStdString( order_->GetName() ) : "";
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface::AddParameter( const QString& objectName, Param_ABC& parameter )
{
    parameters_.push_back( &parameter );
    QWidget* widget = parameter.BuildInterface( objectName, parameter.IsOptional() ? optionalTab_ : mainTab_ );
    widgetToDelete_.push_back( widget );
    parameter.RegisterIn();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface::Draw( gui::GlTools_ABC& tools, ::gui::Viewport_ABC& extent ) const
{
    for( auto it = parameters_.begin() ; it != parameters_.end() ; ++it )
    {
        const geometry::Point2f p = ( entity_ ) ? entity_->Get< kernel::Positions >().GetPosition() : geometry::Point2f( 0.f, 0.f );
        extent.SetHotpoint( p );
        (*it)->Draw( p, extent, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::GetIndex
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
int MissionInterface::GetIndex( Param_ABC* param ) const
{
    for( int i = 0; i < (int)parameters_.size(); ++i )
        if( parameters_[ i ] == param )
            return i;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::HasParameter
// Created: ABR 2013-11-13
// -----------------------------------------------------------------------------
bool MissionInterface::HasParameter( const Param_ABC& parameter ) const
{
    for( auto it = parameters_.begin() ; it != parameters_.end() ; ++it )
        if( ( *it )->HasParameter( parameter ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::IsFilling
// Created: ABR 2013-12-05
// -----------------------------------------------------------------------------
bool MissionInterface::IsFilling() const
{
    return filling_;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface::FixOrigin
// Created: ABR 2014-01-07
// -----------------------------------------------------------------------------
void MissionInterface::FixOrigin( bool fix ) const
{
    for( auto it = parameters_.begin() ; it != parameters_.end() ; ++it )
        ( *it )->FixOrigin( fix );
}
