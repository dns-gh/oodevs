// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoStatusBar.h"
#include "gaming/HumanFactors.h"
#include "gaming/Reinforcements.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "ENT/ENT_Tr.h"
#include "icons.h"
#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: InfoStatusBar constructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoStatusBar::InfoStatusBar( QWidget* parent, kernel::Controllers& controllers )
    : QWidget( parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    setFixedSize( 250, 32 );
    QGridLayout* grid = new QGridLayout( this, 1, 7, 2, 0 );
    AddButton( grid, reinforced_ , 0 );
    AddButton( grid, reinforcing_, 1 );
    AddButton( grid, mood_       , 5 );
    AddButton( grid, experience_ , 6 );
    AddButton( grid, tiredness_  , 7 );

    InitializeIcons();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar destructor
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
InfoStatusBar::~InfoStatusBar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::InitializeIcons
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::InitializeIcons()
{
    // $$$$ SBO 2007-02-07: enumerate order!
    moodIcons_.push_back( MAKE_PIXMAP( morale_fanatic ) );
    moodIcons_.push_back( MAKE_PIXMAP( morale_good ) );
    moodIcons_.push_back( MAKE_PIXMAP( morale_medium ) );
    moodIcons_.push_back( MAKE_PIXMAP( morale_bad ) );

    experienceIcons_.push_back( MAKE_PIXMAP( experience_veteran ) );
    experienceIcons_.push_back( MAKE_PIXMAP( experience_experienced ) );
    experienceIcons_.push_back( MAKE_PIXMAP( experience_rookie ) );

    tirednessIcons_.push_back( MAKE_PIXMAP( tiredness_normal ) );
    tirednessIcons_.push_back( MAKE_PIXMAP( tiredness_tired ) );
    tirednessIcons_.push_back( MAKE_PIXMAP( tiredness_exhausted ) );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::AddButton
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::AddButton( QGridLayout* grid, QLabel*& label, int index )
{
    label = new QLabel( this );
    label->setAlignment( Qt::AlignCenter );
    label->setFixedSize( 32, 32 );
    grid->addWidget( label, 0, index );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::NotifySelected
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !selected_ )
        return;
    if( const HumanFactors* humans = static_cast< const HumanFactors* >( selected_->Retrieve< kernel::HumanFactors_ABC >() ) )
        SetHumanFactors( *humans );
    if( const Reinforcements* reinforcements = selected_->Retrieve< Reinforcements >() )
        SetReinforcements( *reinforcements );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::NotifyUpdated
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::NotifyUpdated( const kernel::HumanFactors_ABC& element )
{
    if( !selected_ || selected_->Retrieve< kernel::HumanFactors_ABC >() != &element )
        return;
    const HumanFactors& humans = static_cast< const HumanFactors& >( element );
    SetHumanFactors( humans );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::NotifyUpdated
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::NotifyUpdated( const Reinforcements& element )
{
    if( !selected_ )
        return;
    const Reinforcements* reinforcements = selected_->Retrieve< Reinforcements >();
    if( reinforcements == &element )
        SetReinforcements( element ); 
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::SetHumanFactors
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::SetHumanFactors( const HumanFactors& humans )
{
    mood_->setPixmap( moodIcons_.at( humans.GetMorale() ) );
    QToolTip::add( mood_, tr( "Morale: " ) + ENT_Tr::ConvertFromUnitMoral( humans.GetMorale() ).c_str() );
    experience_->setPixmap( experienceIcons_.at( humans.GetExperience() ) );
    QToolTip::add( experience_, tr( "Experience: " ) + ENT_Tr::ConvertFromUnitExperience( humans.GetExperience() ).c_str() );
    tiredness_->setPixmap( tirednessIcons_.at( humans.GetTiredness() ) );
    QToolTip::add( tiredness_, tr( "Tiredness: " ) + ENT_Tr::ConvertFromUnitFatigue( humans.GetTiredness() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: InfoStatusBar::SetReinforcements
// Created: SBO 2007-02-15
// -----------------------------------------------------------------------------
void InfoStatusBar::SetReinforcements( const Reinforcements& reinforcements )
{
    if( reinforcements.reinforced_ )
    {
        reinforcing_->setPixmap( MAKE_PIXMAP( reinforcing ) );    
        const QString tip = tr( "Reinforcing unit '%1'." ).arg( reinforcements.reinforced_->GetName() );
        QToolTip::add( reinforcing_, tip );
    }
    else
    {
        reinforcing_->setPixmap( QPixmap() );
        QToolTip::remove( reinforcing_ );
    }
    if( !reinforcements.reinforcements_.empty() )
    {
        reinforced_->setPixmap( MAKE_PIXMAP( reinforced ) );
        QString tip = tr( "Reinforced by: " );
        for( unsigned int i = 0; i < reinforcements.reinforcements_.size(); ++i )
            tip += "\n" + reinforcements.reinforcements_[i]->GetName();
        QToolTip::add( reinforced_, tip );
    }
    else
    {
        reinforced_->setPixmap( QPixmap() );
        QToolTip::remove( reinforced_ );
    }
}
