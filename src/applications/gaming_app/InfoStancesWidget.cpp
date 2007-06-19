// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoStancesWidget.h"
#include "gaming/Attributes.h"
#include "gaming/tools.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "icons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget constructor
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
InfoStancesWidget::InfoStancesWidget( QWidget* parent, kernel::Controllers& controllers )
    : QHBox( parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    layout()->setAlignment( Qt::AlignCenter );
    previous_ = new QPushButton( this );
    previous_->setFixedSize( 32, 32 );
    QLabel* label = new QLabel( this );
    label->setPixmap( MAKE_PIXMAP( stance_arrow ) );
    label->setFixedSize( 10, 32 );
    next_ = new QPushButton( this );
    next_->setFixedSize( 32, 32 );
    progress_ = new QLabel( this );
    progress_->setFixedWidth( 29 );
    InitializeIcons();
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget destructor
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
InfoStancesWidget::~InfoStancesWidget()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::Update
// Created: SBO 2007-02-09
// -----------------------------------------------------------------------------
void InfoStancesWidget::Update( const Attributes& attributes )
{
    show();
    previous_->setPixmap( pixmaps_.at( attributes.nOldPosture_ ) );
    QToolTip::add( previous_, tr( "Stance: previous was: '%1'" )
        .arg( tools::ToString( attributes.nOldPosture_ ) ) );
    next_->setPixmap( pixmaps_.at( attributes.nCurrentPosture_ ) );
    if( attributes.nPostureCompletionPourcentage_ < 100 )
        QToolTip::add( next_, tr( "Stance: next is: '%1'" )
            .arg( tools::ToString( attributes.nCurrentPosture_ ) ) );
    else
        QToolTip::add( next_, tr( "Stance: current is: '%1'" )
            .arg( tools::ToString( attributes.nCurrentPosture_ ) ) );
    progress_->setText( QString::number( attributes.nPostureCompletionPourcentage_ ) + Units::percentage );
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::InitializeIcons
// Created: SBO 2007-02-14
// -----------------------------------------------------------------------------
void InfoStancesWidget::InitializeIcons()
{
    pixmaps_.resize( eNbrUnitPosture );
    pixmaps_[eUnitPosture_PostureMouvement]         = MAKE_PIXMAP( stance_move );
    pixmaps_[eUnitPosture_PostureMouvementDiscret]  = MAKE_PIXMAP( stance_discreet );
    pixmaps_[eUnitPosture_PostureArret]             = MAKE_PIXMAP( stance_stop );
    pixmaps_[eUnitPosture_PosturePosteReflexe]      = MAKE_PIXMAP( stance_reflex );
    pixmaps_[eUnitPosture_PosturePoste]             = MAKE_PIXMAP( stance_posted );
    pixmaps_[eUnitPosture_PosturePosteAmenage]      = MAKE_PIXMAP( stance_equipped );
    pixmaps_[eUnitPosture_PosturePostePrepareGenie] = MAKE_PIXMAP( stance_engineered );
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::NotifySelected
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoStancesWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    if( selected_ != entity )
    {
        selected_ = entity;
        hide();
        if( !selected_ )
            return;
        if( const Attributes* attributes = static_cast< const Attributes* >( selected_->Retrieve< kernel::Attributes_ABC >() ) )
            Update( *attributes );
    }
}

// -----------------------------------------------------------------------------
// Name: InfoStancesWidget::NotifyUpdated
// Created: SBO 2007-03-01
// -----------------------------------------------------------------------------
void InfoStancesWidget::NotifyUpdated( const kernel::Attributes_ABC& extension )
{
    if( selected_ && selected_->Retrieve< kernel::Attributes_ABC >() == &extension )
        Update( static_cast< const Attributes& >( extension ) );
}
