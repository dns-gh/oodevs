// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ExerciseDialog.h"
#include "moc_ExerciseDialog.cpp"
#include "clients_kernel/Controllers.h"
#include <boost/algorithm/string.hpp>
#include "tools/ExerciseConfig.h"
#include <boost/filesystem/path.hpp>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ExerciseDialog constructor
// Created: SBO 2010-03-09
// -----------------------------------------------------------------------------
ExerciseDialog::ExerciseDialog( QWidget* parent, kernel::Controllers& controllers, Exercise& exercise, const tools::ExerciseConfig& config )
    : QDialog( parent, "ExerciseDialog" )
    , controllers_( controllers )
    , exercise_( exercise )
    , config_( config )
{
    setModal( false );
    setCaption( tr( "Exercise" ) );
    Q3GridLayout* grid = new Q3GridLayout( this, 4, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 10 );
    grid->setRowStretch( 2, 5 );
    grid->setRowStretch( 3, 1 );
    grid->setRowStretch( 4, 1 );
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
    }
    {
        Q3GroupBox* box = new Q3VGroupBox( tr( "Briefing" ), this );
        Q3HBox* hbox = new Q3HBox( box );
        new QLabel( tr( "Language" ), hbox );
        lang_ = new gui::ValuedComboBox< QString >( hbox );
        lang_->AddItem( tr( "English" ), "en" );
        lang_->AddItem( tr( "French" ), "fr" );
        lang_->AddItem( tr( "Spanish" ), "es" );
        connect( lang_, SIGNAL( activated( int ) ), this, SLOT( OnChangeLang() ) );
        QPushButton* textFormat = new QPushButton( tr( "source" ), hbox );
        textFormat->setToggleButton( true );
        connect( textFormat, SIGNAL( toggled( bool ) ), this, SLOT( OnToggleDisplayMode( bool ) ) );
        briefing_ = new Q3TextEdit( box );
        briefing_->setTextFormat( Qt::RichText );
        grid->addMultiCellWidget( box, 1, 1, 0, 2 );
    }
    {
        Q3GroupBox* box = new Q3HGroupBox( tr( "Files" ), this );
        resources_ = new Q3ListView( box );
        resources_->addColumn( tr( "Name" ) );
        resources_->addColumn( tr( "File" ) );
        resources_->header()->setMovingEnabled( false );
        resources_->setAllColumnsShowFocus( true );
        resources_->setResizeMode( Q3ListView::LastColumn );
        Q3VBox* tools = new Q3VBox( box );
        QPushButton* add = new QPushButton( tr( "+" ), tools );
        add->setMaximumWidth( 40 );
        connect( add, SIGNAL( clicked() ), this, SLOT( OnAddResource() ) );
        QPushButton* del = new QPushButton( tr( "-" ), tools );
        del->setMaximumWidth( 40 );
        connect( del, SIGNAL( clicked() ), this, SLOT( OnDeleteResource() ) );
        grid->addMultiCellWidget( box, 2, 2, 0, 2 );
    }
    {
        Q3GroupBox* box = new Q3VGroupBox( tr( "Parameters" ), this );
        infiniteDotationsCheckBox_ = new QCheckBox( tr( "Infinite resources" ), box );
        humanEvolutionCheckBox_ = new QCheckBox( tr( "Human factors automatic evolution" ), box );
        grid->addMultiCellWidget( box, 3, 3, 0, 2 );
    }
    {
        Q3HBox* box = new Q3HBox( this );
        QPushButton* ok = new QPushButton( tr( "Ok" ), box );
        QPushButton* cancel = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 4, 2 );
        connect( ok, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog destructor
// Created: SBO 2010-03-09
// -----------------------------------------------------------------------------
ExerciseDialog::~ExerciseDialog()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::NotifyCreated
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::NotifyCreated( const Exercise& exercise )
{
    Update( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::Update
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::Update( const Exercise& exercise )
{
    name_->setText( exercise.GetName() );
    briefings_.clear();
    resources_->clear();
    exercise.Accept( *this );
    OnChangeLang();
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::Visit
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::VisitBriefing( const QString& lang, const QString& text )
{
    briefings_[ lang ] = text;
    if( selectedLang_ == lang )
        briefing_->setText( text );
    else
        briefing_->clear();
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::VisitResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::VisitResource( const QString& name, const QString& file )
{
    AddResource( name, file );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::showEvent
// Created: JSR 2010-05-04
// -----------------------------------------------------------------------------
void ExerciseDialog::showEvent( QShowEvent* showEvent )
{
    QDialog::showEvent( showEvent );
    infiniteDotationsCheckBox_->setChecked( exercise_.GetSettings().GetValue< bool >( "infinite-dotation" ) );
    humanEvolutionCheckBox_->setChecked( exercise_.GetSettings().GetValue< bool >( "human-evolution" ) );
}

namespace
{
    std::string AbsolutePath( const std::string& file )
    {
        QDir info( file.c_str() );
        return info.absPath().ascii();
    }

    QString MakeRelativePath( const QString& filename, const tools::ExerciseConfig& config )
    {
        const std::string sp1 = AbsolutePath( bfs::path( config.GetExerciseFile() ).branch_path().string() );
        const std::string sp2 = bfs::path( filename.ascii() ).string();
        if( boost::istarts_with( sp2, sp1 ) && sp1.size() < sp2.size() )
            return bfs::path( sp2.substr( sp1.length() ) ).relative_path().string().c_str();
        return sp2.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::AddResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::AddResource( const QString& name, const QString& file )
{
    Q3ListViewItem* item = new Q3ListViewItem( resources_ );
    item->setRenameEnabled( 0, true );
    item->setText( 0, name );
    item->setText( 1, MakeRelativePath( file, config_ ) );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnChangeLang
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::OnChangeLang()
{
    briefings_[ selectedLang_ ] = briefing_->text();
    selectedLang_ = lang_->GetValue();
    briefing_->setText( briefings_[ selectedLang_ ] );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnAccept
// Created: SBO 2010-03-09
// -----------------------------------------------------------------------------
void ExerciseDialog::OnAccept()
{
    OnChangeLang();
    exercise_.SetName( name_->text() );
    for( T_Briefings::const_iterator it = briefings_.begin(); it != briefings_.end(); ++it )
        exercise_.SetBriefing( it->first, it->second );
    exercise_.ClearResources();
    for( Q3ListViewItemIterator it( resources_ ); it.current(); ++it )
        exercise_.AddResource( it.current()->text( 0 ), it.current()->text( 1 ) );
    exercise_.GetSettings().SetValue< bool >( "infinite-dotation", infiniteDotationsCheckBox_->isChecked() );
    exercise_.GetSettings().SetValue< bool >( "human-evolution", humanEvolutionCheckBox_->isChecked() );
    accept();
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnReject
// Created: SBO 2010-03-09
// -----------------------------------------------------------------------------
void ExerciseDialog::OnReject()
{
    Update( exercise_ );
    reject();
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnToggleDisplayMode
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::OnToggleDisplayMode( bool toggled )
{
    briefings_[ selectedLang_ ] = briefing_->text();
    briefing_->setTextFormat( toggled ? Qt::PlainText : Qt::RichText );
    briefing_->setText( briefings_[ selectedLang_ ] );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::sizeHint
// Created: SBO 2010-03-09
// -----------------------------------------------------------------------------
QSize ExerciseDialog::sizeHint() const
{
    return QSize( 400, 400 );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnAddResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::OnAddResource()
{
    const QString filename = Q3FileDialog::getOpenFileName( config_.GetExerciseFile().c_str() );
    if( !filename.isNull() )
        AddResource( QFileInfo( filename ).baseName(), filename );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnDeleteResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::OnDeleteResource()
{
    if( Q3ListViewItem* item = resources_->currentItem() )
        delete item;
}
