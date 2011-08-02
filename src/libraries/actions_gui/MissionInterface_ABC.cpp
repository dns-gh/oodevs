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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/OrderType.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "actions_gui/resources.h"

#pragma warning( push, 0 )
#include <QtGui/qtabwidget.h>
#include <Qt3Support/q3scrollview.h>
#pragma warning( pop )

using namespace actions::gui;

namespace
{
    Q3VBox* CreateTab( QTabWidget* parent, const QString& title, bool enabled = true )
    {
        Q3ScrollView* sc = new Q3ScrollView( parent );
        sc->setResizePolicy( Q3ScrollView::AutoOneFit );
        sc->setFrameStyle( Q3Frame::NoFrame );
        Q3VBox* tab = new Q3VBox( sc );
        tab->setMargin( 5 );
        tab->setSpacing( 5 );
        sc->addChild( tab );
        tab->layout()->setAlignment( Qt::AlignTop );
        parent->addTab( sc, title );
        parent->setTabEnabled( sc, enabled );
        return tab;
    }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::MissionInterface_ABC( QWidget* parent, const kernel::OrderType& order, kernel::Entity_ABC& entity, kernel::ActionController& controller )
    : Q3VBox      ( parent )
    , title_     ( order.GetName().c_str() )
    , controller_( controller )
    , entity_    ( entity )
{
    setMinimumSize( 280, 250 ); // $$$$ SBO 2007-04-27:
    CreateTitle( title_ );
    tabs_ = new QTabWidget( this );
    mainTab_ = CreateTab( tabs_, tools::translate( "MissionInterface_ABC", "Mandatory" ) );
    optionalTab_ = CreateTab( tabs_, tools::translate( "MissionInterface_ABC", "Optional" ) );
    {
        const std::string doctrine = order.GetDoctrineInformation();
        const std::string usage = order.GetUsageInformation();
        Q3VBox* helpTab = CreateTab( tabs_, tools::translate( "MissionInterface_ABC", "Help" ), !doctrine.empty() || !usage.empty() );
        if( !doctrine.empty() )
        {
            Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "MissionInteface_ABC", "Doctrine" ), helpTab );
            QLabel* label = new QLabel( doctrine.c_str(), box );
            label->setAlignment( Qt::TextWordWrap );
        }
        if( !usage.empty() )
        {
            Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tools::translate( "MissionInteface_ABC", "Usage" ), helpTab );
            QLabel* label = new QLabel( usage.c_str(), box );
            label->setAlignment( Qt::TextWordWrap );
        }
    }
    CreateOkCancelButtons();
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC destructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MissionInterface_ABC::~MissionInterface_ABC()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->RemoveFromController();
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CheckValidity
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
bool MissionInterface_ABC::CheckValidity()
{
    bool b = true;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
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
// Name: MissionInterface_ABC::CreateOkCancelButtons
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CreateOkCancelButtons()
{
    Q3HBox* box = new Q3HBox( this );
    box->setMargin( 5 );
    box->setSpacing( 5 );
    box->layout()->setAlignment( Qt::AlignRight );
    ok_ = new QPushButton( tools::translate( "MissionInterface_ABC", "Ok" ), box );
    QPushButton* cancel = new QPushButton( tools::translate( "MissionInterface_ABC", "Cancel" ), box );
    ok_->setDefault( true );
    ok_->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    connect( ok_, SIGNAL( clicked() ), SLOT( OnOk() ) );
    connect( cancel, SIGNAL( clicked() ), parent(), SLOT( hide() ) );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::ChangeOkValueButton
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void MissionInterface_ABC::ChangeOkValueButton( bool planningMode )
{ 
   if( ok_ )
   {
       if( planningMode )
          ok_->setText( tools::translate( "MissionInterface_ABC", "Add to planning" ) );
       else
          ok_->setText( tools::translate( "MissionInterface_ABC", "Ok" ) );
   }
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::AddParameter
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
void MissionInterface_ABC::AddParameter( Param_ABC& parameter )
{
    parameters_.push_back( &parameter );
    parameter.BuildInterface( parameter.IsOptional() ? optionalTab_ : mainTab_ );
    parameter.RegisterIn( controller_ );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::CommitTo
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void MissionInterface_ABC::CommitTo( actions::Action_ABC& action ) const
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (*it)->CommitTo( action );
}

// -----------------------------------------------------------------------------
// Name: MissionInterface_ABC::Draw
// Created: AGE 2006-03-31
// -----------------------------------------------------------------------------
void MissionInterface_ABC::Draw( const kernel::GlTools_ABC& tools, kernel::Viewport_ABC& extent ) const
{
    for( CIT_Parameters it = parameters_.begin() ; it != parameters_.end() ; ++it )
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
    parentWidget()->hide();
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

