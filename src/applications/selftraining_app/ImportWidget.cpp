// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ImportWidget.h"
#include "moc_ImportWidget.cpp"
#include "ScenarioEditPage.h"
#include "clients_gui/Tools.h"
#include "frontend/commands.h"
#include "frontend/CreateExercise.h"
#include "tools/GeneralConfig.h"
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qprogressbar.h>
#include <qtabbar.h>
#include <qtabwidget.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

namespace
{
    class TabWidget : public QTabWidget
    {
    public:
        TabWidget( QWidget* parent )
            : QTabWidget( parent )
        {
            setBackgroundOrigin( QWidget::WindowOrigin );
            tabBar()->setBackgroundOrigin( QWidget::WindowOrigin );
            setMargin( 0 );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ImportWidget constructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
ImportWidget::ImportWidget( ScenarioEditPage& page, QWidget* parent, const tools::GeneralConfig& config )
    : QGroupBox( 1, Qt::Vertical, parent )
    , page_( page )
    , config_( config )
{
    setFrameShape( QFrame::NoFrame );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    tabs_ = new TabWidget( this );
    connect( tabs_, SIGNAL( currentChanged( QWidget* ) ), &page, SLOT( UpdateEditButton( QWidget* ) ) );
    {
        QGroupBox* importGroup = new QGroupBox( 2, Qt::Vertical, parent );
        importGroup->setFrameShape( QFrame::NoFrame );
        importGroup->setBackgroundOrigin( QWidget::WindowOrigin );
        importGroup->setMargin( 0 );
        QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, importGroup );
        group->setFrameShape( QFrame::NoFrame );
        group->setBackgroundOrigin( QWidget::WindowOrigin );
        {
            QLabel* label = new QLabel( tools::translate( "ImportWidget", "Package to be installed:" ), group );
            label->setBackgroundOrigin( QWidget::WindowOrigin );
            QHBox* hBox = new QHBox( group );
            hBox->setBackgroundOrigin( QWidget::WindowOrigin );
            package_ = new QLineEdit( hBox );
            package_->setBackgroundOrigin( QWidget::WindowOrigin );
            QButton* browseBtn = new QPushButton( tools::translate( "ImportWidget", "Browse..." ), hBox );
            browseBtn->setBackgroundOrigin( QWidget::WindowOrigin );
            connect( browseBtn, SIGNAL( clicked() ), SLOT( PackageBrowseClicked() ) );
        }
        {
            QLabel* label = new QLabel( tools::translate( "ImportWidget", "Package content:" ), group );
            label->setBackgroundOrigin( QWidget::WindowOrigin );
            label->setAlignment( Qt::AlignTop );
            QVBox* vBox = new QVBox( group );
            vBox->setBackgroundOrigin( QWidget::WindowOrigin );
            vBox->setSpacing( 5 );
            packageName_ = new QLineEdit( vBox );
            packageName_->setBackgroundOrigin( QWidget::WindowOrigin );
            packageName_->setReadOnly( true );
            packageDescription_ = new QTextEdit( vBox );
            packageDescription_->setBackgroundOrigin( QWidget::WindowOrigin );
            packageDescription_->setMaximumHeight( 80 );
            packageDescription_->setReadOnly( true );
            packageContent_ = new QListBox( vBox );
            packageContent_->setBackgroundOrigin( QWidget::WindowOrigin );
        }
        packageProgress_ = new QProgressBar( importGroup );
        packageProgress_->hide();
        tabs_->addTab( importGroup, tools::translate( "ImportWidget", "Package" ) );
    }
    // LTO begin
    {
        QGroupBox* hbox = new QGroupBox( 1, Qt::Horizontal, parent );
        hbox->setFrameShape( QFrame::NoFrame );
        hbox->setBackgroundOrigin( QWidget::WindowOrigin );
        hbox->setInsideSpacing( 10 );
        AddModelChoice( hbox );
        AddInput( hbox );
        AddOutput( hbox );
        AddTerrainChoice( hbox );
        tabs_->addTab( hbox, tools::translate( "ImportWidget", "Scenario" ) );
    }
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: ImportWidget destructor
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
ImportWidget::~ImportWidget()
{
    // NOTHING
}

namespace
{
    struct Progress
    {
        Progress( QProgressBar* progress ) : progress_( progress ), count_( 0 ) {}
        void operator()()
        {
            progress_->setProgress( ++count_ );
            qApp->processEvents();
        }
        QProgressBar* progress_;
        unsigned count_;
    };
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::InstallExercise
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void ImportWidget::InstallExercise()
{
    if( tabs_->currentPageIndex() == 0 ) // OTPAK
        InstallPackage();
    else if( tabs_->currentPageIndex() == 1 ) // LTO
        ImportScenario(); // LTO
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::EnableEditButton
// Created: JSR 2010-07-19
// -----------------------------------------------------------------------------
bool ImportWidget::EnableEditButton()
{
    if( tabs_->currentPageIndex() == 0 ) // OTPAK
        return packageContent_->count() != 0;
    else // LTO
        return ( !terrain_.isEmpty() && !outputScenario_.isEmpty() && !inputEdit_->text().isEmpty() ); // LTO
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::InstallPackage
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void ImportWidget::InstallPackage()
{
    if( packageContent_->count() )
    {
        zip::izipfile archive( package_->text() );
        if( archive.isOk() )
        {
            packageProgress_->show();
            packageProgress_->setProgress( 0, packageContent_->count() );
            setCursor( QCursor::waitCursor );
            frontend::commands::InstallPackageFile( archive, config_.GetRootDir(), Progress( packageProgress_ ) );
            setCursor( QCursor::arrowCursor );
            packageProgress_->hide();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::ImportScenario
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::ImportScenario()
{
    if( !terrain_.isEmpty() && !outputScenario_.isEmpty() && !inputEdit_->text().isEmpty() )
    {
        const QStringList model = QStringList::split( "/", model_ );
        frontend::CreateExercise( config_, outputScenario_.ascii(), terrain_.ascii(), model.front().ascii(), model.back().ascii() );
        page_.LaunchScenarioImport( inputEdit_->text(), outputScenario_ );
        // $$$$ Wait for import to complete, and then launch prepa or have import app launch prepa itself.
        //page_.LaunchPreparation( outputScenario_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::ReadPackageContentFile
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
bool ImportWidget::ReadPackageContentFile()
{
    zip::izipfile archive( package_->text() );
    if( !archive.isOk() )
        return false;
    zip::izipstream zipStream( archive, "content.xml" );
    if( zipStream.bad() )
        return false;
    try
    {
        std::string name, description;
        xml::xistreamstream xis( zipStream );
        xis >> xml::start( "content" )
                >> xml::content( "name", name )
                >> xml::content( "description", description )
            >> xml::end();
        packageName_->setText( name.c_str() );
        packageDescription_->setText( description.c_str() );
    }
    catch( xml::exception& )
    {
        return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::AddModelChoice
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::AddModelChoice( QGroupBox* box )
{
    QGroupBox* hbox = new QGroupBox( 1, Qt::Vertical, box );
    hbox->setBackgroundOrigin( QWidget::WindowOrigin );
    QLabel* label = new QLabel( tools::translate( "ImportWidget", "Model: " ), hbox );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QComboBox* editModelList = new QComboBox( hbox );
    editModelList->setBackgroundOrigin( QWidget::WindowOrigin );
    connect( editModelList, SIGNAL( activated ( const QString & ) ), SLOT( OnModelChanged( const QString & ) ) );
    editModelList->insertItem( tools::translate( "ImportWidget", "Model:" ) );
    QStringList decisionalModels = frontend::commands::ListModels( config_ );
    for( QStringList::const_iterator it = decisionalModels.begin(); it != decisionalModels.end(); ++it )
    {
        const QStringList physicalModels = frontend::commands::ListPhysicalModels( config_, (*it).ascii() );
        for( QStringList::const_iterator itP = physicalModels.begin(); itP != physicalModels.end(); ++itP )
            editModelList->insertItem( QString( "%1/%2" ).arg( *it ).arg( *itP ) );
    }
    if( editModelList->count() == 2 )
        editModelList->setCurrentItem( 1 );
    editModelList->setShown( editModelList->count() > 2 );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::AddOutput
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::AddOutput( QGroupBox* box )
{
    QHBox* scenarioBox = new QHBox( box );
    scenarioBox->setBackgroundOrigin( QWidget::WindowOrigin );
    QLabel* label = new QLabel( tools::translate( "ImportWidget", "Output Scenario Name: " ), scenarioBox );
    scenarioBox->setStretchFactor( label, 1 );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QLineEdit* editName = new QLineEdit( tools::translate( "ImportWidget", "Scenario Name" ), scenarioBox );
    scenarioBox->setStretchFactor( editName, 1 );
    editName->setBackgroundOrigin( QWidget::WindowOrigin );
    connect( editName, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnOutputName( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::AddInput
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::AddInput( QGroupBox* box )
{
    QHBox* hbox = new QHBox( box );
    hbox->setBackgroundOrigin( QWidget::WindowOrigin );
    QLabel* label = new QLabel( tools::translate( "ImportWidget", "Input Scenario: " ), hbox );
    label->setBackgroundOrigin( QWidget::WindowOrigin );
    QHBox* browseBox = new QHBox( hbox );
    browseBox->setBackgroundOrigin( QWidget::WindowOrigin );
    inputEdit_ = new QLineEdit( browseBox );
    inputEdit_->setBackgroundOrigin( QWidget::WindowOrigin );
    QButton* browseBtn = new QPushButton( tools::translate( "ImportWidget", "Browse..." ), browseBox );
    browseBtn->setBackgroundOrigin( QWidget::WindowOrigin );
    connect( browseBtn, SIGNAL( clicked() ), SLOT( OnChangeScenario() ) );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::AddTerrainChoice
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::AddTerrainChoice( QGroupBox* box )
{
    QComboBox* editTerrainList = new QComboBox( box );
    editTerrainList->setBackgroundOrigin( QWidget::WindowOrigin );
    connect( editTerrainList, SIGNAL( activated( const QString& ) ), SLOT( OnTerrainChanged( const QString& ) ) );
    editTerrainList->clear();
    editTerrainList->insertItem( tools::translate( "ImportWidget", "Terrain:" ) );
    editTerrainList->insertStringList( frontend::commands::ListTerrains( config_ ) );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::PackageBrowseClicked
// Created: JSR 2010-07-13
// -----------------------------------------------------------------------------
void ImportWidget::PackageBrowseClicked()
{
    const QString filename = QFileDialog::getOpenFileName( "", "Officer Training packages (*.otpak)", this, "", tools::translate( "ImportWidget", "Select a package" ) );
    SelectPackage( filename );
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::OnModelChanged
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::OnModelChanged( const QString & model )
{
    model_ = model;
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::OnTerrainChanged
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::OnTerrainChanged( const QString& terrain )
{
    terrain_ = terrain;
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::OnOutputName
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::OnOutputName( const QString& scenario )
{
    outputScenario_ = scenario;
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::OnChangeScenario
// Created: JSR 2010-07-13
// LTO
// -----------------------------------------------------------------------------
void ImportWidget::OnChangeScenario()
{
    const QString filename = QFileDialog::getOpenFileName( QString::null, "*.xml", this );
    inputEdit_->setText( filename );
    page_.UpdateEditButton();
}

// -----------------------------------------------------------------------------
// Name: ImportWidget::SelectPackage
// Created: SBO 2011-03-30
// -----------------------------------------------------------------------------
void ImportWidget::SelectPackage( const QString& filename )
{
    package_->setText( filename );
    if( !filename.isEmpty() )
    {
        packageName_->setText( "" );
        packageDescription_->setText( "" );
        packageContent_->clear();
        if( ReadPackageContentFile() )
            packageContent_->insertStringList( frontend::commands::ListPackageFiles( package_->text().ascii() ) );
        else
            packageName_->setText( tools::translate( "ImportWidget", "otpak corrupted: unable to load content properly" ) );
    }
    page_.UpdateEditButton();
}
