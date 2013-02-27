// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "actions_gui/resources.h"
#include "MissionInterface_ABC.h"
#include "moc_MissionInterface_ABC.cpp"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"
#include "ParamComboBox.h"

#include "tools/ExerciseConfig.h"
#include <boost/filesystem.hpp>

#include <QtWebKit/qwebview.h>

namespace bfs = boost::filesystem;

using namespace actions::gui;

namespace
{
    Q3VBox* CreateTab( QTabWidget* parent, const QString& title, bool enabled = true )
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
}

namespace
{
    void CreateLineField( const QString& name, const QString& text, QVBoxLayout* parentLayout )
    {
        QTextEdit* textEdit = new QTextEdit( text );
        textEdit->setReadOnly( true );
        QGroupBox* group = new QGroupBox( name );
        QVBoxLayout* layout = new QVBoxLayout( group );
        layout->addWidget( textEdit );
        parentLayout->addWidget( group );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( QWidget* parent, const kernel::OrderType& order, kernel::Entity_ABC& entity, kernel::Controllers& controllers, const tools::ExerciseConfig& config, std::string missionSheetPath /*=""*/ )
    : Q3VBox            ( parent )
    , ParamInterface_ABC()
    , title_     ( order.GetName().c_str() )
    , controllers_( controllers )
    , entity_    ( entity )
{
    setMinimumSize( 280, 250 );
    CreateTitle( title_ );
    tabs_ = new QTabWidget( this );
    mainTab_ = CreateTab( tabs_, tr( "Mandatory" ) );
    optionalTab_ = CreateTab( tabs_, tr( "Optional" ) );
    {
        std::string path = config.GetPhysicalChildPath( missionSheetPath );
        std::string doctrine = order.GetDoctrineInformation();
        std::string usage = order.GetUsageInformation();
        std::string fileName = std::string( config.GetPhysicalChildPath( missionSheetPath ) + "/" + order.GetName() + ".html" );
        if( bfs::is_regular_file( fileName ) || ( !doctrine.empty() && !usage.empty() ) )
        {
            QWidget* helpTab = new QWidget( tabs_ );
            tabs_->addTab( helpTab, tr( "Help" ) );
            QVBoxLayout* helpLayout = new QVBoxLayout( helpTab );

            if( bfs::is_regular_file( fileName ) )
            {
                std::replace( fileName.begin(), fileName.end(), '\\', '/' );
                QWebView* missionSheetText = new QWebView();
                missionSheetText->load( QUrl( fileName.c_str() ) );
                helpLayout->addWidget( missionSheetText );
            }
            else 
            {
                if( !doctrine.empty() )
                    CreateLineField( tr( "Doctrine" ), doctrine.c_str(), helpLayout );
                if( !usage.empty() )
                    CreateLineField( tr( "Usage" ), usage.c_str(), helpLayout );
            }
        }
    }
    {
        Q3HBox* box = new Q3HBox( this );
        box->setMargin( 5 );
        box->setSpacing( 5 );
        box->layout()->setAlignment( Qt::AlignRight );
        ok_ = new QPushButton( tr( "Ok" ), box );
        QPushButton* cancel = new QPushButton( tr( "Cancel" ), box );
        ok_->setDefault( true );
        ok_->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
        connect( ok_, SIGNAL( clicked() ), SLOT( OnOk() ) );
        connect( cancel, SIGNAL( clicked() ), parent, SLOT( Close() ) );
    }
    controllers_.modes_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->RemoveFromController();
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
    controllers_.modes_.Unregister( *this );
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
// Name: MissionInterface_ABC::IsEmpty
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::IsEmpty() const
{
    return parameters_.empty();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::GetEntity
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& MissionInterface_ABC::GetEntity() const
{
    return entity_;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CreateTitle
// Created: SBO 2007-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateTitle( const QString& title )
{
    paintEvent( 0 , title);
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddParameter( Param_ABC& parameter )
{
    parameters_.push_back( &parameter );
    parameter.BuildInterface( parameter.IsOptional() ? optionalTab_ : mainTab_ );
    parameter.RegisterIn( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CommitTo( actions::Action_ABC& action ) const
{
    for( auto it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->CommitTo( action );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Draw( gui::GlTools_ABC& tools, ::gui::Viewport_ABC& extent ) const
{
    for( auto it = parameters_.begin() ; it != parameters_.end() ; ++it )
    {
        const geometry::Point2f p = entity_.Get< kernel::Positions >().GetPosition();
        extent.SetHotpoint( p );
        (*it)->Draw( p, extent, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::OnOk
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
void MissionInterface_ABC::OnOk()
{
    if( !CheckValidity() )
        return;
    Publish();
    emit( OkClicked() );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Title
// Created: AGE 2008-07-15
// -----------------------------------------------------------------------------
QString MissionInterface_ABC::Title() const
{
    return title_;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::paintEvent
// Created: FPT 2011-06-28
// -----------------------------------------------------------------------------
void MissionInterface_ABC::paintEvent( QPaintEvent*, QString title )
{
    Q3HBox* box = new Q3HBox( this );
    box->setPaletteBackgroundColor( Qt::white );
    box->setFixedHeight( 32 );
    QLabel* label = new QLabel( " " + title, box );
    QFont font = label->font();
    font.setBold( true );
    font.setPixelSize( 16 );
    label->setFont( font );
    label = new QLabel( box );
    label->setPixmap( MAKE_PIXMAP( mission_title ) );
    label->setAlignment( Qt::AlignRight );
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
// Name: MissionInterface_ABC::NotifyModeChanged
// Created: ABR 2013-02-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::NotifyModeChanged( E_Modes newMode )
{
    ModesObserver_ABC::NotifyModeChanged( newMode );
    if( !ok_ )
        return;
    ok_->setText( newMode == eModes_Planning ? tr( "Add to planning" ) : tr( "Ok" ) );
}
