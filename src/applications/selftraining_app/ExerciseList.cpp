// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "frontend/commands.h" 
#include "ExerciseList.h"
#include "MenuButton.h" 
#include "SideList.h" 
#include "moc_ExerciseList.cpp" 
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"

#include "qtextbrowser.h"
#include "qfileinfo.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp> 

#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const std::string& subDir /*= ""*/, bool showBrief )
    : QVBox   ( parent ) 
    , config_ ( config ) 
    , subDir_ ( subDir )
    , showBrief_ ( showBrief ) 
{
    // HBox 
    QHBox* box = new QHBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setSpacing( 50 );
    
    {   
        // leftBox 
        QLabel* label ; 
        QVBox* leftBox = new QVBox( box );
        leftBox->setSpacing( 5 );
        leftBox->setBackgroundOrigin( QWidget::WindowOrigin );
        label = new QLabel(tools::translate("ScenarioLauncherPage","Exercise:") , leftBox); 
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        exercises_ = new QListBox( leftBox );

        label = new QLabel(tools::translate("ScenarioLauncherPage","Sides:") , leftBox ); 
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        sides_ = new SideList( leftBox, config );
        leftBox->setStretchFactor( exercises_, 3 );
        leftBox->setStretchFactor( sides_, 1 );

        connect( exercises_, SIGNAL( highlighted( const QString& ) ), this,  SLOT( UpdateExercise( const QString& ) ) );
        connect( exercises_, SIGNAL( doubleClicked( QListBoxItem* ) ), this, SLOT( SelectExercise() ) );
    }
    
    if ( showBrief ) 
    {
        // RightBox 
        QVBox* rightBox = new QVBox( box );

        rightBox->setMinimumWidth( 200 ) ; 
        rightBox->setBackgroundOrigin( QWidget::WindowOrigin );
        rightBox->setSpacing( 5 );
        briefingImage_ = new QLabel( rightBox );
        briefingImage_->setBackgroundOrigin( QWidget::WindowOrigin );
        briefingText_ = new QTextEdit( rightBox );
        briefingText_->setBackgroundOrigin( QWidget::WindowOrigin );
        briefingText_->setFont( QFont( "Georgia", 10, QFont::Normal, true ) ) ; 
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::~ExerciseList()
{
    // NOTHING
}

// =============================================================================
// SLOTS 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ExerciseList::Update
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::Update()
{
    exercises_->clear();
    const QImage pix( "resources/images/selftraining/mission.png" );
    QStringList list = frontend::commands::ListExercises( config_, subDir_ );
    for( QStringList::iterator it = list.begin(); it != list.end(); ++it )
        exercises_->insertItem( pix, *it );
    exercises_->setSelected( 0, true );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::UpdateExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::UpdateExercise( const QString& exercise )
{
     emit( Highlight( GetHighlight() ) ); 
    // update sides
    sides_->Update( QString( subDir_.c_str() ) + "/"  + exercise ); 
    // update briefing 
    if ( showBrief_ ) 
    {
        xml::xifstream xis( config_.GetExerciseFile( subDir_ + "/"  + exercise.ascii() ) );
        briefingText_->setText( tr("No available briefing") ); 
        briefingText_->hide(); 
        std::string image ; 
        xis >> xml::start( "exercise" )
                >> xml::optional() >> xml::start( "meta" )
                    >> xml::optional() >> xml::start( "briefing" )
                        >> xml::optional()  >> xml::content("image", image )
                            >> xml::list( "text", *this, &ExerciseList::ReadBriefingText );
        const std::string imagePath = config_.GetExerciseDir( exercise.ascii() ) + "/" + image ; 
        const QImage pix( imagePath.c_str() ) ; 
        briefingImage_->setPixmap( pix ) ; 
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::GetHighlight
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
const QString ExerciseList::GetHighlight()
{
    if ( exercises_->selectedItem() ) 
        return QString( subDir_.c_str() ) + "/" +  exercises_->selectedItem()->text() ; 
    else
        return "" ; 
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::ReadBriefingText
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void ExerciseList::ReadBriefingText( xml::xistream& xis )
{
    std::string lang, text ; 
    xis >> xml::attribute("lang", lang ) 
        >> text ; 
    if ( lang == tools::translate( "General", "Lang" ).ascii() ) 
    {
        briefingText_->setText( text.c_str() ) ; 
        briefingText_->show(); 
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::SelectExercise( )
{
    if( exercises_->selectedItem() )
        emit( Select( GetHighlight() ) ) ; 
}

