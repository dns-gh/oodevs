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
#include <boost/filesystem/convenience.hpp>

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
    , resources_( new QStandardItemModel( this) )
    , orderFiles_( new QStandardItemModel( this) )
{
    setModal( false );
    setCaption( tr( "Exercise" ) );
    QGridLayout* grid = new QGridLayout( this );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 10 );
    grid->setRowStretch( 2, 5 );
    grid->setRowStretch( 3, 1 );
    grid->setRowStretch( 4, 1 );
    {
        QGroupBox* box = new QGroupBox( tr( "Information" ), this );
        QHBoxLayout* layout = new QHBoxLayout();
        box->setLayout( layout );
        layout->addWidget( new QLabel( tr( "Name:" ) ) );
        name_ = new QLineEdit( box );
        layout->addWidget( name_ );
        grid->addWidget( box, 0, 0, 1, 2 );
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Briefing" ), this );
        QVBoxLayout* vLayout = new QVBoxLayout();
        box->setLayout( vLayout );
        QHBoxLayout* hLayout = new QHBoxLayout();
        vLayout->addLayout( hLayout );
        hLayout->addWidget( new QLabel( tr( "Language" ) ) );
        lang_ = new gui::ValuedComboBox< QString >();
        hLayout->addWidget( lang_ );
        lang_->AddItem( tr( "English" ), "en" );
        lang_->AddItem( tr( "French" ), "fr" );
        lang_->AddItem( tr( "Spanish" ), "es" );
        connect( lang_, SIGNAL( activated( int ) ), this, SLOT( OnChangeLang() ) );
        QPushButton* textFormat = new QPushButton( tr( "source" ) );
        hLayout->addWidget( textFormat );
        textFormat->setToggleButton( true );
        connect( textFormat, SIGNAL( toggled( bool ) ), this, SLOT( OnToggleDisplayMode( bool ) ) );
        briefing_ = new QTextEdit();
        vLayout->addWidget( briefing_ );
        briefing_->setTextFormat( Qt::RichText );
        grid->addWidget( box, 1, 0, 1, 2 );
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Files" ) );
        QHBoxLayout* layout = new QHBoxLayout();
        box->setLayout( layout );
        resourcesView_ = new QTreeView();
        layout->addWidget( resourcesView_ );
        resourcesView_->setModel( resources_ );
        resources_->setColumnCount( 2 );
        QStringList headers;
        headers << tr( "Name" ) << tr( "File" );
        resources_->setHorizontalHeaderLabels( headers );
        resourcesView_->header()->setMovable( false );
        QVBoxLayout* vLayout = new QVBoxLayout();
        layout->addLayout( vLayout );
        QPushButton* add = new QPushButton( tr( "+" ) );
        vLayout->addWidget( add );
        add->setMaximumWidth( 40 );
        connect( add, SIGNAL( clicked() ), this, SLOT( OnAddResource() ) );
        QPushButton* del = new QPushButton( tr( "-" ) );
        vLayout->addWidget( del );
        del->setMaximumWidth( 40 );
        connect( del, SIGNAL( clicked() ), this, SLOT( OnDeleteResource() ) );
        grid->addWidget( box, 2, 0, 1, 2 );
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Order files" ) );
        QHBoxLayout* layout = new QHBoxLayout();
        box->setLayout( layout );
        orderFilesView_ = new QTreeView();
        layout->addWidget( orderFilesView_ );
        orderFilesView_->setModel( orderFiles_ );
        QStringList headers( tr( "File" ) );
        orderFiles_->setHorizontalHeaderLabels( headers );
        orderFilesView_->header()->setMovable( false );
        QVBoxLayout* vLayout = new QVBoxLayout();
        layout->addLayout( vLayout );
        QPushButton* add = new QPushButton( tr( "+" ) );
        vLayout->addWidget( add );
        add->setMaximumWidth( 40 );
        connect( add, SIGNAL( clicked() ), this, SLOT( OnAddOrderFile() ) );
        QPushButton* del = new QPushButton( tr( "-" ) );
        vLayout->addWidget( del );
        del->setMaximumWidth( 40 );
        connect( del, SIGNAL( clicked() ), this, SLOT( OnDeleteOrderFile() ) );
        grid->addWidget( box, 3, 0, 1, 2 );
    }
    {
        QGroupBox* box = new QGroupBox( tr( "Parameters" ) );
        QVBoxLayout* vLayout = new QVBoxLayout();
        box->setLayout( vLayout );
        infiniteDotationsCheckBox_ = new QCheckBox( tr( "Infinite resources" ) );
        vLayout->addWidget( infiniteDotationsCheckBox_ );
        humanEvolutionCheckBox_ = new QCheckBox( tr( "Human factors automatic evolution" ) );
        vLayout->addWidget( humanEvolutionCheckBox_ );
        grid->addWidget( box, 4, 0, 1, 2 );
    }
    {
        QHBoxLayout* box = new QHBoxLayout();
        QPushButton* ok = new QPushButton( tr( "Ok" ) );
        QPushButton* cancel = new QPushButton( tr( "Cancel" ) );
        box->addWidget( ok );
        box->addWidget( cancel );
        grid->addLayout( box, 5, 1 );
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
    resources_->removeRows( 0, resources_->rowCount() );
    orderFiles_->removeRows( 0, orderFiles_->rowCount() );;
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
// Name: ExerciseDialog::VisitOrderFile
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void ExerciseDialog::VisitOrderFile( const QString& file )
{
    AddOrderFile( file );
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
        return info.absPath().toStdString();
    }

    QString MakeRelativePath( const QString& filename, const tools::ExerciseConfig& config )
    {
        const std::string sp1 = AbsolutePath( bfs::path( config.GetExerciseFile() ).branch_path().string() );
        const std::string sp2 = bfs::path( filename.toStdString() ).string();
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
    QStandardItem* item1 = new QStandardItem( name );
    QStandardItem* item2 = new QStandardItem( MakeRelativePath( file, config_ ) );
    item1->setEditable( false );
    item2->setEditable( false );
    QList< QStandardItem* > items;
    items << item1 << item2;
    resources_->appendRow( items );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::AddOrderFile
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void ExerciseDialog::AddOrderFile( const QString& file )
{
    const QString filename = MakeRelativePath( file, config_ );
    if( orderFiles_->findItems( filename ).size() > 0 )
        return;
    orderFiles_->appendRow( new QStandardItem( filename ) );
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

namespace
{
    QString CopyIfAbsolute( const QString& filename, const tools::ExerciseConfig& config, QStringList& list )
    {
        try
        {
            bfs::path file( filename.toStdString() );
            if( !file.is_complete() )
                return filename;
            list.append( file.filename().string().c_str() );
            bfs::path dest = bfs::path( config.BuildExerciseChildFile( "scripts") ) / "resources";
            bfs::create_directories( dest );
            bfs::copy_file( file, dest / file.filename() );
            return MakeRelativePath( ( dest / file.filename() ).string().c_str(), config );
        }
        catch( ... )
        {
            return filename;
        }
    }
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
    for( int row = 0; row < resources_->rowCount(); ++row )
        exercise_.AddResource( resources_->item( row )->text(), resources_->item( row, 1 )->text() );
    exercise_.ClearOrderFiles();
    QStringList copiedFiles;
    for( int row = 0; row < orderFiles_->rowCount(); ++row )
        exercise_.AddOrderFile( CopyIfAbsolute( orderFiles_->item( row )->text(), config_, copiedFiles ) );
    if( !copiedFiles.isEmpty() )
    {
        QString message = tr( "The following files will be copied to the exercise scripts/resources subfolder:\n" );
        QStringListIterator it( copiedFiles );
        while( it.hasNext() )
            message.append( "\n- " + it.next() );
        QMessageBox::information( this, tr( "Exercise" ), message );
    }
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
    const QString filename = QFileDialog::getOpenFileName( this, QString(), config_.GetExerciseFile().c_str() );
    if( !filename.isNull() )
        AddResource( QFileInfo( filename ).baseName(), filename );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnDeleteResource
// Created: SBO 2010-03-11
// -----------------------------------------------------------------------------
void ExerciseDialog::OnDeleteResource()
{
    const QModelIndex index = resourcesView_->currentIndex();
    if( index.isValid() )
        resources_->removeRow( index.row(), index.parent() );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnAddOrderFile
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void ExerciseDialog::OnAddOrderFile()
{
    const QString filename = QFileDialog::getOpenFileName( this, QString(), QString(), tr( "Order files (*.ord)" ) );
    if( !filename.isNull() )
        AddOrderFile( filename );
}

// -----------------------------------------------------------------------------
// Name: ExerciseDialog::OnDeleteOrderFile
// Created: JSR 2012-03-01
// -----------------------------------------------------------------------------
void ExerciseDialog::OnDeleteOrderFile()
{
    const QModelIndex index = orderFilesView_->currentIndex();
    if( index.isValid() )
        orderFiles_->removeRow( index.row(), index.parent() );
}
