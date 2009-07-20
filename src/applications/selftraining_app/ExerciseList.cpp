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
#include "ExerciseLister_ABC.h"
#include "moc_ExerciseList.cpp"
#include "MenuButton.h" 
#include "Profile.h"
#include "ProfileList.h"
#include "frontend/commands.h" 
#include "tools/GeneralConfig.h"
#include "clients_gui/Tools.h"

#include <qfileinfo.h>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#pragma warning( push )
#pragma warning( disable: 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const ExerciseLister_ABC& lister, const std::string& subDir /*= ""*/, bool showBrief /*= true*/, bool showProfile /*=true*/ )
    : QVBox      ( parent ) 
    , config_    ( config ) 
    , subDir_    ( subDir )
    , showBrief_ ( showBrief )
    , lister_    ( lister )
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

        label = new QLabel( tools::translate( "ExerciseList", "Profile:" ), leftBox );
        label->setBackgroundOrigin( QWidget::WindowOrigin );
        profiles_ = new ProfileList( leftBox, config );
        leftBox->setStretchFactor( exercises_, 3 );
        leftBox->setStretchFactor( profiles_, 1 );
        label->setShown( showProfile );
        profiles_->setShown( showProfile );

        connect( profiles_ , SIGNAL( Select( const Profile& ) ), this, SLOT( SelectProfile( const Profile& ) ) );
        connect( exercises_, SIGNAL( highlighted( int ) ), this, SLOT( SelectExercise( int ) ) );
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
    QApplication::postEvent( this, new QCustomEvent( 4242 ) );
}

namespace
{
    QString MakePath( const std::string& dir, const std::string& file )
    {
        if( dir.empty() )
            return file.c_str();
        return QString( "%1/%2" ).arg( dir.c_str() ).arg( file.c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::SelectExercise( int index )
{
    const QString exercise = *exercisesList_.at( index ); 
    profiles_->Update( MakePath( subDir_, exercise.ascii() ) );
    if( showBrief_ )
    {
        briefingText_->setText( tools::translate( "ExerciseList", "No briefing available" ) );
        briefingText_->hide();
        try
        {
            xml::xifstream xis( config_.GetExerciseFile( MakePath( subDir_, exercise.ascii() ).ascii() ) );
            std::string image;
            xis >> xml::start( "exercise" )
                    >> xml::optional() >> xml::start( "meta" )
                        >> xml::optional() >> xml::start( "briefing" )
                            >> xml::optional() >> xml::content("image", image )
                            >> xml::optional()
                                >> xml::start( "texts" )
                                    >> xml::list( "text", *this, &ExerciseList::ReadBriefingText )
                                >> xml::end();
            const std::string imagePath = config_.GetExerciseDir( MakePath( exercise.ascii(), image ).ascii() );
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
        xml::xifstream xis( config_.GetExerciseFile( MakePath( subDir_, exercise.ascii() ).ascii() ) );
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
// Name: ExerciseList::BuildExercisePath
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
QString ExerciseList::BuildExercisePath() const
{
    if( exercises_->selectedItem() )
    {
        const QString exercise( *exercisesList_.at( exercises_->index( exercises_->selectedItem() ) ) );
        return MakePath( subDir_, exercise.ascii() );
    }
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
// Name: ExerciseList::SelectProfile
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ExerciseList::SelectProfile( const Profile& profile )
{
    if( exercises_->selectedItem() )
        emit Select( BuildExercisePath(), profile.GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Clear
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseList::Clear()
{
    QApplication::postEvent( this, new QCustomEvent( 4243 ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::customEvent
// Created: SBO 2008-11-05
// -----------------------------------------------------------------------------
void ExerciseList::customEvent( QCustomEvent* e )
{
    if( e->type() == 4242 )
    {
        static const QImage pix( "resources/images/selftraining/mission.png" );
        lister_.ListExercises( exercisesList_ );
        exercises_->clear();
        for( QStringList::iterator it = exercisesList_.begin(); it != exercisesList_.end(); ++it )
            exercises_->insertItem( pix, GetExerciseDisplayName( *it ) );
        exercises_->setSelected( 0, true );
    }
    else if( e->type() == 4243 )
    {
        profiles_->clear();
        exercises_->clear();
    }
}
