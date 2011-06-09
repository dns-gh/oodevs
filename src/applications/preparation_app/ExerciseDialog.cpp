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
#include <qvgroupbox.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ExerciseDialog constructor
// Created: SBO 2010-03-09
// -----------------------------------------------------------------------------
ExerciseDialog::ExerciseDialog( QWidget* parent, kernel::Controllers& controllers, Exercise& exercise, const tools::ExerciseConfig& config, bool& infiniteDotations )
    : QDialog( parent, "ExerciseDialog" )
    , controllers_( controllers )
    , exercise_( exercise )
    , config_( config )
    , infiniteDotations_( infiniteDotations )
{
    setModal( false );
    setCaption( tr( "Exercise" ) );
    QGridLayout* grid = new QGridLayout( this, 4, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 10 );
    grid->setRowStretch( 2, 5 );
    grid->setRowStretch( 3, 1 );
    grid->setRowStretch( 4, 1 );
    {
        QGroupBox* box = new QHGroupBox( tr( "Information" ), this );
        new QLabel( tr( "Name:" ), box );
        name_ = new QLineEdit( box );
        grid->addMultiCellWidget( box, 0, 0, 0, 2 );
    }
    {
        QGroupBox* box = new QVGroupBox( tr( "Briefing" ), this );
        QHBox* hbox = new QHBox( box );
        new QLabel( tr( "Language" ), hbox );
        lang_ = new gui::ValuedComboBox< QString >( hbox );
        lang_->AddItem( tr( "English" ), "en" );
        lang_->AddItem( tr( "French" ), "fr" );
        lang_->AddItem( tr( "Spanish" ), "es" );
        connect( lang_, SIGNAL( activated( int ) ), this, SLOT( OnChangeLang() ) );
        QPushButton* textFormat = new QPushButton( tr( "source" ), hbox );
        textFormat->setToggleButton( true );
        connect( textFormat, SIGNAL( toggled( bool ) ), this, SLOT( OnToggleDisplayMode( bool ) ) );
        briefing_ = new QTextEdit( box );
        briefing_->setTextFormat( Qt::RichText );
        grid->addMultiCellWidget( box, 1, 1, 0, 2 );
    }
    {
        QGroupBox* box = new QHGroupBox( tr( "Files" ), this );
        resources_ = new QListView( box );
        resources_->addColumn( tr( "Name" ) );
        resources_->addColumn( tr( "File" ) );
        resources_->header()->setMovingEnabled( false );
        resources_->setAllColumnsShowFocus( true );
        resources_->setResizeMode( QListView::LastColumn );
        QVBox* tools = new QVBox( box );
        QButton* add = new QPushButton( tr( "+" ), tools );
        add->setMaximumWidth( 40 );
        connect( add, SIGNAL( clicked() ), this, SLOT( OnAddResource() ) );
        QButton* del = new QPushButton( tr( "-" ), tools );
        del->setMaximumWidth( 40 );
        connect( del, SIGNAL( clicked() ), this, SLOT( OnDeleteResource() ) );
        grid->addMultiCellWidget( box, 2, 2, 0, 2 );
    }
    {
        QGroupBox* box = new QVGroupBox( tr( "Parameters" ), this );
        infiniteDotationsCB_ = new QCheckBox( tr( "Infinite resources" ), box );
        grid->addMultiCellWidget( box, 3, 3, 0, 2 );
    }
    {
        QHBox* box = new QHBox( this );
        QButton* ok = new QPushButton( tr( "Ok" ), box );
        QButton* cancel = new QPushButton( tr( "Cancel" ), box );
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
    infiniteDotationsCB_->setChecked( infiniteDotations_ );
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
        if( boost::istarts_with( sp2, sp1 ) )
            return bfs::path( sp2.substr( 0, sp1.length() ) ).relative_path().string().c_str();
        return sp2.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::AddResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::AddResource( const QString& name, const QString& file )
{
    QListViewItem* item = new QListViewItem( resources_ );
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
    for( QListViewItemIterator it( resources_ ); it.current(); ++it )
        exercise_.AddResource( it.current()->text( 0 ), it.current()->text( 1 ) );
    infiniteDotations_ = infiniteDotationsCB_->isChecked();
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
    const QString filename = QFileDialog::getOpenFileName( config_.GetExerciseFile().c_str() );
    if( !filename.isNull() )
        AddResource( QFileInfo( filename ).baseName(), filename );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnDeleteResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::OnDeleteResource()
{
    if( QListViewItem* item = resources_->currentItem() )
        delete item;
}
