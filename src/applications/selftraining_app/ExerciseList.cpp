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
#include "ExerciseProperties.h"
#include "ProfileList.h"
#include "ExerciseListView.h"
#include "frontend/LocalExerciseFilter.h"
#include "frontend/Profile.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/Controllers.h"

namespace
{
    static int clearEvent = 4242;
}

namespace frontend
{
    Exercise_ABC;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList constructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::ExerciseList( QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers,
                            bool showBrief /* = true*/, bool showProfile /* =true*/, bool showParams /* = true*/, bool enableParams /* = true*/ )
    : gui::LanguageChangeObserver_ABC< QWidget >( parent )
    , controllers_      ( controllers )
    , filter_           ( 0 )
    , defaultFilter_    ( new frontend::LocalExerciseFilter() )
{
    //left box
    exerciseLabel_ = new QLabel();

    profileLabel_ = new QLabel();

    QWidget* leftBox = new QWidget();
    QVBoxLayout* leftBoxLayout = new QVBoxLayout();
    {
        exercises_ = new ExerciseListView( leftBox, config, fileLoader );
        connect( exercises_, SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( SelectExercise( const QModelIndex& ) ) );
        profiles_ = new ProfileList( leftBox, config, fileLoader );
        profiles_->setShown( showProfile );
        connect( profiles_ , SIGNAL( Select( const frontend::Profile& ) ), this, SLOT( SelectProfile( const frontend::Profile& ) ) );
    }
    leftBoxLayout->addWidget( exerciseLabel_ );
    leftBoxLayout->addWidget( exercises_, 3 );
    leftBoxLayout->addWidget( profileLabel_ );
    leftBoxLayout->addWidget( profiles_, 1 );
    leftBoxLayout->setSpacing( 5 );

    //box
    QHBoxLayout* mainBoxLayout = new QHBoxLayout( this );
    mainBoxLayout->addLayout(leftBoxLayout );
    mainBoxLayout->setMargin( 5 );
    mainBoxLayout->setSpacing( 50 );
    {
        properties_ = new ExerciseProperties( this, parent, config, fileLoader, showBrief, showParams, enableParams );

        connect( properties_, SIGNAL( ExercisePropertiesChanged() ), this, SLOT( OnExercisePropertiesChanged() ) );
    }
    mainBoxLayout->addWidget( properties_ );
    profileLabel_->setShown( showProfile );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
ExerciseList::~ExerciseList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ExerciseList::OnLanguageChanged()
{
    exerciseLabel_->setText( tools::translate( "ExerciseList", "Exercise:" ) );
    profileLabel_->setText( tools::translate( "ExerciseList", "Profile:" ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyCreated
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void ExerciseList::NotifyCreated( const frontend::Exercise_ABC& exercise )
{
    if( !filter_ && defaultFilter_->Allows( exercise ) || filter_ && filter_->Allows( exercise ) )
        UpdateExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyUpdated
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ExerciseList::NotifyUpdated( const frontend::Exercise_ABC& exercise )
{
    UpdateExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::NotifyDeleted
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void ExerciseList::NotifyDeleted( const frontend::Exercise_ABC& exercise )
{
    exercises_->DeleteExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SetFilter
// Created: SBO 2010-10-21
// -----------------------------------------------------------------------------
void ExerciseList::SetFilter( const frontend::ExerciseFilter_ABC& filter )
{
    filter_ = &filter;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectExercise
// Created: RDS 2008-08-27
// -----------------------------------------------------------------------------
void ExerciseList::SelectExercise( const QModelIndex& index )
{
    if( const frontend::Exercise_ABC* selected = GetSelectedExercise() )
    {
        profiles_->Update( exercises_->GetExerciseName( index ) );
        properties_->Select( selected );
        emit Select( *selected, frontend::Profile() );
    }
    else
    {
        profiles_->Update( "" );
        properties_->Select( 0 );
        emit ClearSelection();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::GetSelectedExercise
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
const frontend::Exercise_ABC* ExerciseList::GetSelectedExercise() const
{
    return exercises_->GetSelectedExercise();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::SelectProfile
// Created: SBO 2008-10-31
// -----------------------------------------------------------------------------
void ExerciseList::SelectProfile( const frontend::Profile& profile )
{
    if( const frontend::Exercise_ABC* exercise = GetSelectedExercise() )
        emit Select( *exercise, profile );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Clear
// Created: LDC 2008-10-24
// -----------------------------------------------------------------------------
void ExerciseList::Clear()
{
    emit ClearSelection();
    QApplication::postEvent( this, new QEvent( static_cast< QEvent::Type >( ::clearEvent ) ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::customEvent
// Created: SBO 2008-11-05
// -----------------------------------------------------------------------------
void ExerciseList::customEvent( QEvent* e )
{
    if( e->type() == ::clearEvent )
    {
        profiles_->clear();
        exercises_->Clear();
        properties_->Update();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::UpdateExerciseEntry
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ExerciseList::UpdateExerciseEntry( const frontend::Exercise_ABC& exercise )
{
    const bool isAllowed = !filter_ && defaultFilter_->Allows( exercise ) || filter_ && filter_->Allows( exercise );
    if( exercises_->FindExerciseItem( exercise ) != 0 )
    {
        if( !isAllowed )
            exercises_->DeleteExerciseEntry( exercise );
    }
    else if( isAllowed )
        exercises_->AddExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::Exists
// Created: SBO 2010-04-15
// -----------------------------------------------------------------------------
bool ExerciseList::Exists( const QString& exercise ) const
{
    return exercises_->Exists( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::IsPropertiesValid
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
bool ExerciseList::IsPropertiesValid() const
{
    return properties_->IsValid();
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::ChangeExerciceParameters
// Created: SLG 2010-07-08
// -----------------------------------------------------------------------------
bool ExerciseList::ChangeExerciceParameters()
{
    if( const frontend::Exercise_ABC* exercise = GetSelectedExercise() )
        return properties_->Commit( *exercise );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ExerciseList::OnExercisePropertiesChanged
// Created: LGY 2012-06-07
// -----------------------------------------------------------------------------
void ExerciseList::OnExercisePropertiesChanged()
{
    emit ExercisePropertiesChanged();
}
