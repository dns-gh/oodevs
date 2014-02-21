// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExportWidget.h"
#include "moc_ExportWidget.cpp"
#include "ExerciseListView.h"

#include "clients_gui/FileDialog.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/tools.h"

#include "frontend/commands.h"
#include "frontend/ListViewHelper.h"

#include "tools/GeneralConfig.h"
#include "tools/Loader_ABC.h"
#include "tools/VersionHelper.h"
#include "tools/FileWrapper.h"

#include <tools/Helpers.h>
#include <tools/Zip.h>

#include <xeumeuleu/xml.h>

namespace fc = frontend::commands;

// -----------------------------------------------------------------------------
// Name: ExportWidget constructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::ExportWidget( Application& app, QWidget* parent, const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, kernel::Controllers& controllers )
    : gui::WidgetLanguageObserver_ABC< QWidget >( parent )
    , config_     ( config )
    , fileLoader_ ( fileLoader )
    , controllers_( controllers )
{
    //-------eTabs_Exercise------//
    //exercise description
    exerciseDescriptionLabel_ = new QLabel();
    exerciseDescription_ = new QTextEdit();
    exerciseDescription_->setMaximumHeight( 30 );
    QHBoxLayout* exerciseDescriptionLayout = new QHBoxLayout();
    exerciseDescriptionLayout->addWidget( exerciseDescriptionLabel_ );
    exerciseDescriptionLayout->addWidget( exerciseDescription_ );
    exerciseDescriptionLayout->setStretch( 0, 1 );
    exerciseDescriptionLayout->setStretch( 1, 4 );

    //exercise list
    exerciseLabel_ = new QLabel();
    exerciseList_ = new ExerciseListView( app, config, fileLoader );
    exerciseList_->setSelectionMode( QAbstractItemView::ExtendedSelection );
    connect( exerciseList_->selectionModel(),
        SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ),
        SLOT( OnSelectionChanged( const QItemSelection&, const QItemSelection& ) ) );
    QHBoxLayout* exerciseListLayout = new QHBoxLayout();
    exerciseListLayout->addWidget( exerciseLabel_ );
    exerciseListLayout->addWidget( exerciseList_ );
    exerciseListLayout->setStretch( 0, 1 );
    exerciseListLayout->setStretch( 1, 4 );

    //package content
    packageContentLabel_ = new QLabel();
    exerciseContent_ = new QTreeView();
    exerciseContent_->setModel( &exerciseContentModel_ );
    exerciseContent_->setHeaderHidden( true );
    exerciseContent_->setEditTriggers( 0 );
    exerciseContent_->adjustSize();
    exerciseContent_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    QHBoxLayout* packageContentLayout = new QHBoxLayout();
    packageContentLayout->addWidget( packageContentLabel_ );
    packageContentLayout->addWidget( exerciseContent_ );
    packageContentLayout->setStretch( 0, 1 );
    packageContentLayout->setStretch( 1, 4 );

    //exercise layout
    QWidget* exerciseTab = new QWidget();
    QVBoxLayout* exerciseLayout = new QVBoxLayout( exerciseTab );
    exerciseLayout->addLayout( exerciseDescriptionLayout );
    exerciseLayout->addLayout( exerciseListLayout );
    exerciseLayout->addLayout( packageContentLayout );

    //-------eTabs_Terrain-------//
    //terrain description
    terrainDescriptionLabel_ = new QLabel();
    terrainDescription_ = new QTextEdit();
    terrainDescription_->setMaximumHeight( 30 );
    QHBoxLayout* terrainDescriptionLayout = new QHBoxLayout();
    terrainDescriptionLayout->addWidget( terrainDescriptionLabel_ );
    terrainDescriptionLayout->addWidget( terrainDescription_ );
    terrainDescriptionLayout->setStretch( 0, 1 );
    terrainDescriptionLayout->setStretch( 1, 4 );

    //terrain list
    terrainLabel_ = new QLabel();
    terrainList_ = new QListWidget();
    terrainList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( terrainList_, SIGNAL( itemClicked( QListWidgetItem* ) ), SLOT( OnSelectionChanged( QListWidgetItem* ) ) );
    QHBoxLayout* terrainListLayout = new QHBoxLayout();
    terrainListLayout->addWidget( terrainLabel_ );
    terrainListLayout->addWidget( terrainList_ );
    terrainListLayout->setStretch( 0, 1 );
    terrainListLayout->setStretch( 1, 4 );

    //exercise layout
    QWidget* terrainTab = new QWidget();
    QVBoxLayout* terrainLayout = new QVBoxLayout( terrainTab );
    terrainLayout->addLayout( terrainDescriptionLayout );
    terrainLayout->addLayout( terrainListLayout );

    //------eTabs_Models------//
    //model names
    modelsNameLabel_ = new QLabel();
    modelName_ = new QLineEdit();
    modelName_->setMaximumHeight( 30 );
    connect( modelName_, SIGNAL( textEdited( const QString& ) ), SLOT( OnModelNameChanged( const QString& ) ) );
    QHBoxLayout* modelNameLayout = new QHBoxLayout();
    modelNameLayout->addWidget( modelsNameLabel_ );
    modelNameLayout->addWidget( modelName_ );
    modelNameLayout->setStretch( 0, 1 );
    modelNameLayout->setStretch( 1, 4 );

    //model description
    modelsDescriptionLabel_ = new QLabel();
    modelDescription_ = new QTextEdit();
    modelDescription_->setMaximumHeight( 30 );
    QHBoxLayout* modelDescriptionLayout = new QHBoxLayout();
    modelDescriptionLayout->addWidget( modelsDescriptionLabel_ );
    modelDescriptionLayout->addWidget( modelDescription_ );
    modelDescriptionLayout->setStretch( 0, 1 );
    modelDescriptionLayout->setStretch( 1, 4 );

    //model decisional
    modelsDecisionalLabel_ = new QLabel();
    decisionalCheckBox_ = new QCheckBox();
    decisionalCheckBox_->setEnabled( false );
    QHBoxLayout* modelDecisionalLayout = new QHBoxLayout();
    modelDecisionalLayout->addWidget( modelsDecisionalLabel_ );
    modelDecisionalLayout->addWidget( decisionalCheckBox_ );
    modelDecisionalLayout->setStretch( 0, 1 );
    modelDecisionalLayout->setStretch( 1, 4 );

    //model physical
    modelsPhysicalLabel_ = new QLabel();
    physicalList_ = new QListWidget();
    physicalList_->setFont( QFont( "Calibri", 12, QFont::Bold ) );
    connect( physicalList_, SIGNAL( itemClicked( QListWidgetItem* ) ), SLOT( OnSelectionChanged( QListWidgetItem* ) ) );
    QHBoxLayout* modelPhysicalLayout = new QHBoxLayout();
    modelPhysicalLayout->addWidget( modelsPhysicalLabel_ );
    modelPhysicalLayout->addWidget( physicalList_ );
    modelPhysicalLayout->setStretch( 0, 1 );
    modelPhysicalLayout->setStretch( 1, 4 );

    //exercise layout
    QWidget* modelTab = new QWidget();
    QVBoxLayout* modelLayout = new QVBoxLayout( modelTab );
    modelLayout->addLayout( modelNameLayout );
    modelLayout->addLayout( modelDescriptionLayout );
    modelLayout->addLayout( modelDecisionalLayout );
    modelLayout->addLayout( modelPhysicalLayout );
    modelLayout->setStretch( 0, 1 );
    modelLayout->setStretch( 1, 4 );

    //------general panel------/
    //general tab container
    tabs_ = new QTabWidget( this );
    tabs_->addTab( exerciseTab, "" );
    tabs_->addTab( terrainTab, "" );
    tabs_->addTab( modelTab, "" );

    //general progress bar
    progress_ = new QProgressBar( this );
    progress_->hide();

    //main layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( tabs_ );
    layout->addWidget( progress_ );

    //general configuration
    package_.first = config_.GetRootDir();
    controllers_.Register( *this );
    connect( tabs_, SIGNAL( currentChanged( int ) ), SLOT( OnButtonChanged() ) );
    Update();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget destructor
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
ExportWidget::~ExportWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void ExportWidget::OnLanguageChanged()
{
    tabs_->setTabText( eTabs_Exercise,  tools::translate( "ExportWidget", "Exercise" ) );
    tabs_->setTabText( eTabs_Terrain,   tools::translate( "ExportWidget", "Terrain" ) );
    tabs_->setTabText( eTabs_Models,    tools::translate( "ExportWidget", "Models" ) );

    exerciseDescriptionLabel_->setText( tools::translate( "ExportWidget", "Package description:" ) );
    exerciseLabel_->setText(            tools::translate( "ExportWidget", "Exercise:" ) );
    packageContentLabel_->setText(      tools::translate( "ExportWidget", "Package content:" ) );
    terrainDescriptionLabel_->setText(  tools::translate( "ExportWidget", "Package description:" ) );
    terrainLabel_->setText(             tools::translate( "ExportWidget", "Terrain:" ) );
    modelsNameLabel_->setText(          tools::translate( "ExportWidget", "Model name:" ) );
    modelsDescriptionLabel_->setText(   tools::translate( "ExportWidget", "Package description:" ) );
    modelsDecisionalLabel_->setText(    tools::translate( "ExportWidget", "Decisional model:" ) );
    modelsPhysicalLabel_->setText(      tools::translate( "ExportWidget", "Physical model:" ) );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentSelection
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
QString ExportWidget::GetCurrentSelection() const
{
    QString result;
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        if( const QStandardItem* item = exerciseList_->GetSelectedExerciseItem() )
            result = item->text();
        break;
    case eTabs_Terrain:
        if( QListWidgetItem* item = terrainList_->currentItem() )
            result = item->text();
        break;
    case eTabs_Models:
        if( !modelName_->text().isEmpty() )
            if( QListWidgetItem* item = physicalList_->currentItem() )
                result = item->text();
        break;
    default:
        break;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentDescription
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
QTextEdit* ExportWidget::GetCurrentDescription() const
{
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        return exerciseDescription_;
    case eTabs_Terrain:
        return terrainDescription_;
    case eTabs_Models:
        return modelDescription_;
    default:
        break;
    }
    assert( false );
    return 0;
}

namespace
{
    std::pair< tools::Path, tools::Path > Extract( const QString& text )
    {
        std::string selectedText = text.toStdString();
        size_t separator = selectedText.find_first_of( '/' );
        std::string base = selectedText.substr( 0, separator );
        std::string physical = selectedText.substr( separator, std::string::npos );
        return std::make_pair( tools::Path::FromUTF8( base ), tools::Path::FromUTF8( physical ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::GetCurrentPackage
// Created: LGY 2012-06-14
// -----------------------------------------------------------------------------
QString ExportWidget::GetCurrentPackage() const
{
    const QString text = GetCurrentSelection();
    if( tabs_->currentIndex() == eTabs_Models )
        return modelName_->text() + Extract( text ).second.ToUTF8().c_str();
    return text;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::IsButtonEnabled
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ExportWidget::IsButtonEnabled()
{
    QString text = GetCurrentSelection();
    if( !text.isEmpty() )
        package_.second = tools::Path::FromUnicode( text.toStdWString() ) + ".otpak";
    return !text.isEmpty();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnSelectionChanged
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::OnSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected )
{
    const auto selections = selected.indexes();
    for( auto it = selections.begin(); it != selections.end(); ++it )
    {
        const tools::Path exercise = exerciseList_->GetExerciseName( *it );
        if( !exercise.IsEmpty() )
        {
            frontend::BuildExerciseFeatures( exercise, config_, exerciseContentModel_ );
            frontend::BuildExerciseData( exercise, config_, exerciseContentModel_, fileLoader_ );
            exerciseContent_->expandAll();
            exerciseContent_->resizeColumnToContents( 0 );
        }
    }
    const auto deselections = deselected.indexes();
    for( auto it = deselections.begin(); it != deselections.end(); ++it )
    {
        const tools::Path exercise = exerciseList_->GetExerciseName( *it );
        if( !exercise.IsEmpty() )
        {
            frontend::RemoveExerciseFeatures( exercise, exerciseContentModel_ );
            std::vector< tools::Path > exercises;
            const auto indexes = exerciseList_->selectionModel()->selectedIndexes();
            for( auto it = indexes.begin(); it != indexes.end(); ++it )
                exercises.push_back( exerciseList_->GetExerciseName( *it ) );
            frontend::RemoveExerciseData( exercise, exercises, config_, exerciseContentModel_, fileLoader_ );
        }
    }
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnSelectionChanged
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::OnSelectionChanged( QListWidgetItem* item )
{
    Update( item );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnModelNameChanged
// Created: LGY 2012-06-14
// -----------------------------------------------------------------------------
void ExportWidget::OnModelNameChanged( const QString& /*text*/ )
{
    OnButtonChanged();
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::Update
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::Update( QListWidgetItem* item )
{
    if( !item ) // No item, refresh lists
    {
        // Terrain
        terrainList_->clear();
        terrainList_->addItems( fc::PathListToQStringList( fc::ListTerrains( config_ ) ) );
        // Physical
        decisionalCheckBox_->setEnabled( false );
        physicalList_->clear();
        QStringList physicalBase;
        tools::Path::T_Paths decisionalModels = fc::ListModels( config_ );
        for( auto it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
        {
            const tools::Path::T_Paths physicalModels = fc::ListPhysicalModels( config_, *it );
            for( auto itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
                physicalBase << QString( "%1/%2" ).arg( it->ToUTF8().c_str() ).arg( itP->ToUTF8().c_str() );
        }
        physicalList_->addItems( physicalBase );
        modelName_->clear();
    }
    else if( tabs_->currentIndex() == eTabs_Models )
    {
        modelName_->setText( Extract( physicalList_->currentItem()->text() ).first.ToUTF8().c_str() );
        decisionalCheckBox_->setEnabled( true );
    }
    OnButtonChanged();
}

namespace
{
    void AddProgress( QProgressBar* progress, int value )
    {
        progress->setValue( std::min( 98, progress->value() + value ) );
        qApp->processEvents();
    }

    void BrowseDirectory( const tools::Path& root, const tools::Path& name, tools::zip::OutputArchive& archive, bool recursive, QProgressBar* progress )
    {
        for( auto it = root.begin(); it != root.end(); ++it )
        {
            const tools::Path& file = it->FileName();
            if( it->IsRegularFile() )
            {
                archive.StoreFile( name / file, root / file );
                AddProgress( progress, 10 );
            }
            else if( recursive && it->IsDirectory() )
                BrowseDirectory( *it, name / file, archive, recursive, progress );
            AddProgress( progress, 2 );
        }
    }

    void Serialize( const tools::Path& base, const tools::Path& name, tools::zip::OutputArchive& archive, bool recursive, QProgressBar* progress, const tools::Path& exportName = "" )
    {
        const tools::Path root = base / name;
        if( !root.Exists() )
            return;
        if( !root.IsDirectory() )
            {
                archive.StoreFile( name, root );
                AddProgress( progress, 10 );
            }
        else
            BrowseDirectory( root, exportName != "" ? exportName : name, archive, recursive, progress );
    }

    void BrowseChildren( const tools::Path& base, QStandardItem* item, tools::zip::OutputArchive& archive, QProgressBar* progress, bool recursive )
    {
        int row = 0;
        while( row < item->rowCount() )
        {
            frontend::CheckListItem* child = dynamic_cast< frontend::CheckListItem* >( item->child( row ) );
            if( child && child->checkState() == Qt::Checked )
            {
                tools::Path file = tools::Path::FromUnicode( child->text().toStdWString() ) ;
                Serialize( base, file, archive, recursive, progress );
            }
            AddProgress( progress, 2 );
            ++row;
        }
    }

    void BrowseFiles( const tools::Path& base, const QStandardItemModel& model, tools::zip::OutputArchive& archive, QProgressBar* progress )
    {
        for( int row = 0; row < model.rowCount(); ++row )
        {
            if( model.item( row )->hasChildren() )
            {
                QStandardItem* treeNode = model.item( row );
                for( int row2 = 0; row2 < treeNode->rowCount(); ++row2 )
                {
                    frontend::CheckListItem* item = dynamic_cast< frontend::CheckListItem* >( treeNode->child( row2 ) );
                    if( item && item->checkState() == Qt::Checked )
                    {
                        const tools::Path file = tools::Path::FromUnicode( item->text().toStdWString() );
                        Serialize( base, file, archive, item->IsRecursive(), progress );
                        if( item->hasChildren() )
                            BrowseChildren( base, item, archive, progress, true );
                    }
                    AddProgress( progress, 2 );
                }
            }
        }
    }

    int ListViewSize( const QStandardItemModel& model, QStandardItem* parent  = 0 )
    {
        if( !parent )
            parent = model.invisibleRootItem();

        int itemCount = parent->rowCount();
        if( parent->hasChildren() )
            for( int row = 0; row < parent->rowCount(); ++row )
            itemCount += parent->hasChildren() ? ListViewSize( model, parent->child( row ) ) : 0 ;
        return itemCount;
    }

    tools::Path GetDiffPath( tools::Path basepath, tools::Path other )
    {
        tools::Path diff;
        while( other != basepath )
        {
            diff = other.FileName() / diff;
            other = other.Parent();
        }
        return diff;
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::ExportPackage
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::ExportPackage()
{
    if( BrowseClicked() )
    {
        tools::zip::OutputArchive archive( package_.first / package_.second );
        ExportPackage( archive );
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::BrowseClicked
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
bool ExportWidget::BrowseClicked()
{
    const tools::Path filename = gui::FileDialog::getSaveFileName( this, tools::translate( "ExportWidget", "Select a package" ), package_.second, "SWORD packages (*.otpak)" );
    if( filename.IsEmpty() )
        return false;
    package_.first = filename.Parent();
    package_.second = filename.FileName();
    if( filename.Exists() )
        return QMessageBox::question( this, tools::translate( "ExportWidget", "Overwrite File?" ),
                            tools::translate( "ExportWidget", "%1 already exists.\nDo you want to replace it?" ).arg( QString::fromStdWString( filename.ToUnicode().c_str() ) ),
                            QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::WriteContent
// Created: JSR 2010-07-15
// -----------------------------------------------------------------------------
void ExportWidget::WriteContent( tools::zip::OutputArchive& archive ) const
{
    const QString text = GetCurrentSelection();
    if( text.isEmpty() )
        return;
    const QString package = GetCurrentPackage();
    std::string description = GetCurrentDescription()->toPlainText().toStdString();
    if( description.empty() )
        description = "Packaged scenario of " + package.toStdString() + ".";
    archive.WriteFile( "content.xml",
        [&]( std::ostream& s )
        {
            xml::xostreamstream( s )
                << xml::start( "content" )
                    << xml::content( "name", package.toStdString() )
                    << xml::content( "description", description )
                    << xml::content( "version", tools::AppProjectVersion() );
        } );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::ExportPackage
// Created: ABR 2011-11-03
// -----------------------------------------------------------------------------
void ExportWidget::ExportPackage( tools::zip::OutputArchive& archive )
{
    progress_->show();
    setCursor( Qt::WaitCursor );
    WriteContent( archive );
    switch( tabs_->currentIndex() )
    {
    case eTabs_Exercise:
        ExportExercise( archive );
        break;
    case eTabs_Terrain:
        ExportTerrain( archive );
        break;
    case eTabs_Models:
        ExportModels( archive );
    default:
        break;
    }
    setCursor( Qt::ArrowCursor );
    progress_->hide();
}

void ExportWidget::ExportExercise( tools::zip::OutputArchive& archive )
{
    progress_->setValue( 0 );
    progress_->setMaximum( ListViewSize( exerciseContentModel_ ) );
    BrowseFiles( config_.GetRootDir(), exerciseContentModel_, archive, progress_ );
}

void ExportWidget::ExportTerrain( tools::zip::OutputArchive& archive )
{
    assert( terrainList_->currentItem() );
    progress_->setValue( 0 );
    progress_->setMaximum( 100 );
    const tools::Path diffPath =
        GetDiffPath( config_.GetRootDir(),
            config_.GetTerrainDir( tools::Path::FromUnicode( terrainList_->currentItem()->text().toStdWString() ) ) );
    Serialize( config_.GetRootDir(), diffPath, archive, true, progress_ );
    progress_->setValue( 100 );
}

void ExportWidget::ExportModels( tools::zip::OutputArchive& archive )
{
    assert( physicalList_->currentItem() );
    std::pair< tools::Path, tools::Path > content( Extract( physicalList_->currentItem()->text() ) );

    tools::Path diffPath = GetDiffPath( config_.GetRootDir(), config_.GetModelsDir() ) / content.first;
    tools::Path exportPath = GetDiffPath( config_.GetRootDir(), config_.GetModelsDir() ) / tools::Path::FromUnicode( modelName_->text().toStdWString() );

    if( decisionalCheckBox_->isChecked() )
    {
        progress_->setValue( 0 );
        progress_->setMaximum( 100 );
        Serialize( config_.GetRootDir(), diffPath / "physical" / content.second, archive, true, progress_, exportPath / "physical" / content.second );
        progress_->setValue( 50 );
        Serialize( config_.GetRootDir(), diffPath / "decisional", archive, true, progress_, exportPath / "decisional" );
        progress_->setValue( 100 );
    }
    else
    {
        progress_->setValue( 0 );
        progress_->setMaximum( 100 );
        Serialize( config_.GetRootDir(), diffPath / "physical" / content.second, archive, true, progress_, exportPath / "physical" / content.second );
        progress_->setValue( 100 );
    }
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::NotifyCreated
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::NotifyCreated( const frontend::Exercise_ABC& exercise )
{
    exerciseList_->AddExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::NotifyDeleted
// Created: LGY 2012-05-30
// -----------------------------------------------------------------------------
void ExportWidget::NotifyDeleted( const frontend::Exercise_ABC& exercise )
{
    exerciseList_->DeleteExerciseEntry( exercise );
}

// -----------------------------------------------------------------------------
// Name: ExportWidget::OnButtonChanged
// Created: BAX 2012-10-24
// -----------------------------------------------------------------------------
void ExportWidget::OnButtonChanged()
{
    emit ButtonChanged( IsButtonEnabled(), tools::translate( "Page_ABC", "Export" ) );
}
