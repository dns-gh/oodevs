// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseList.h"
#include "moc_ExerciseList.cpp"
#include "MenuButton.h" 
#include "SideList.h" 
#include "frontend/commands.h" 
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"

#include <qfileinfo.h>
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
    QHBox* box = new QHBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->setSpacing( 50 );
    
    {   
        QVBox* leftBox = new QVBox( box );
        leftBox->setSpacing( 5 );
        leftBox->setBackgroundOrigin( QWidget::WindowOrigin );
        QLabel* label = new QLabel( tools::translate( "ExerciseList", "Exercise:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        exercises_ = new QListBox( leftBox );

        label = new QLabel( tools::translate( "ExerciseList", "Sides:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        sides_ = new SideList( leftBox, config );
        leftBox->setStretchFactor( exercises_, 3 );
        leftBox->setStretchFactor( sides_, 1 );

        connect( exercises_, SIGNAL( highlighted( int ) ), this,  SLOT( UpdateExercise( int ) ) );
        connect( exercises_, SIGNAL( doubleClicked( QListBoxItem* ) ), this, SLOT( SelectExercise() ) );
    }
    
    if( showBrief ) 
    {
        QVBox* rightBox = new QVBox( box );
        rightBox->setMinimumWidth( 200 );
        rightBox->setBackgroundOrigin( QWidget::WindowOrigin );
        rightBox->setSpacing( 5 );
        briefingImage_ = new QLabel( rightBox );
        briefingImage_->setBackgroundOrigin( QWidget::WindowOrigin );
        briefingText_ = new QTextEdit( rightBox );
        briefingText_->setBackgroundOrigin( QWidget::WindowOrigin );
        briefingText_->setFont( QFont( "Georgia", 10, QFont::Normal, true ) );
        briefingText_->setReadOnly( true );
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

// -----------------------------------------------------------------------------
// Name: ExerciseList::Update
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::Update()
{    
    static const QImage pix( "resources/images/selftraining/mission.png" );
    exercisesList_ = frontend::commands::ListExercises( config_, subDir_ );
    exercises_->clear();
    for( QStringList::iterator it = exercisesList_ .begin(); it != exercisesList_ .end(); ++it )
        exercises_->insertItem( pix, GetExerciseDisplayName( *it ) );
    exercises_->setSelected( 0, true );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::UpdateExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::UpdateExercise( int index )
{
    QString exercise = *exercisesList_.at( index ) ; 
    emit Highlight( GetHighlight() );
    sides_->Update( QString( subDir_.c_str() ) + "/"  + exercise );
    if( showBrief_ )
    {
        briefingText_->setText( tools::translate( "ExerciseList", "No briefing available" ) );
        briefingText_->hide();
        try
        {
            xml::xifstream xis( config_.GetExerciseFile( subDir_ + "/"  + exercise.ascii() ) );
            std::string image;
            xis >> xml::start( "exercise" )
                    >> xml::optional() >> xml::start( "meta" )
                        >> xml::optional() >> xml::start( "briefing" )
                            >> xml::optional()  >> xml::content("image", image )
                                >> xml::list( "text", *this, &ExerciseList::ReadBriefingText );
            const std::string imagePath = config_.GetExerciseDir( exercise.ascii() ) + "/" + image;
            const QImage pix( imagePath.c_str() );
            briefingImage_->setPixmap( pix );
        }
        catch( ... )
        {
            // $$$$ SBO 2008-10-07: error in exercise.xml meta, just don't show briefing
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::GetExerciseDisplayName
// Created: RDS 2008-10-02
// -----------------------------------------------------------------------------
QString ExerciseList::GetExerciseDisplayName( const QString& exercise ) const
{
    std::string displayName( exercise.ascii() );
    try
    {
        xml::xifstream xis( config_.GetExerciseFile( subDir_ + "/"  + exercise.ascii() ) );
        xis >> xml::start( "exercise" )
                >> xml::optional() >> xml::start( "meta" )
                    >> xml::optional() >> xml::content( "name", displayName );
    }
    catch( ... )
    {
        // $$$$ SBO 2008-10-07: error in exercise.xml meta, just show directory name
    }
    return displayName.c_str();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::GetHighlight
// Created: RDS 2008-09-01
// -----------------------------------------------------------------------------
const QString ExerciseList::GetHighlight() const
{
    if ( exercises_->selectedItem() )
        return QString( subDir_.c_str() ) + "/" +  *exercisesList_.at( exercises_->index( exercises_->selectedItem() ) );
    else
        return "";
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::ReadBriefingText
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
void ExerciseList::ReadBriefingText( xml::xistream& xis )
{
    std::string lang, text;
    xis >> xml::attribute( "lang", lang )
        >> text;
    if( lang == tools::translate( "General", "Lang" ).ascii() )
    {
        briefingText_->setText( text.c_str() );
        briefingText_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::SelectExercise()
{
    if( exercises_->selectedItem() )
        emit Select( GetHighlight() );
}
