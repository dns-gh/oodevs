// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SoundPanel.h"
#include "moc_SoundPanel.cpp"
#include "FileDialog.h"
#include "RichSlider.h"
#include "RichGroupBox.h"
#include "RichPushButton.h"
#include "SoundManager.h"
#include "Tools.h"
#include "tools/GeneralConfig.h"


using namespace gui;

namespace
{
    QWidget* CreateSlider( const QString& name, const QString& labelName, QWidget* parent )
    {
        QLabel* label = new QLabel( labelName );

        QSlider* slider = new RichSlider( name );
        slider->setRange( 0, 100 );
        slider->setOrientation( Qt::Horizontal );
        slider->setMaximumWidth( 200 );
        slider->setValue( 50 );
        parent->connect( slider, SIGNAL( valueChanged( int ) ), parent, SLOT( OnSliderChanged( int ) ) );

        QWidget* sliderBox = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout( sliderBox );
        layout->addWidget( label, 1 );
        layout->addWidget( slider, 1 );
        return sliderBox;
    }
}

// -----------------------------------------------------------------------------
// Name: SoundPanel constructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
SoundPanel::SoundPanel( QWidget* parent )
    : PreferencePanel_ABC( parent, "SoundPanel" )
{
    QLabel* coordinateLabel = new QLabel( tools::translate( "SoundPanel", "Ajust all sound volumes:" ) );
    RichGroupBox* box = new RichGroupBox( "soundControl", tools::translate( "SoundPanel", "Sound control" ), this );

    QLabel* soundDirectoryLabel = new QLabel( tools::translate( "SoundPanel", "Sound directory :" ) );
    tools::Path soundsPath;
    soundDirectoryEditor_ = new gui::RichLineEdit( "soundDirectoryEditor", soundsPath.Absolute( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) ).Normalize().ToUTF8().c_str() );
    QPushButton* soundDirectoryButton = new gui::RichPushButton( "soundDirectoryButton", tools::translate( "SoundPanel", "..." ) );
    connect( soundDirectoryButton, SIGNAL( clicked( bool ) ), this, SLOT( OnChooseSoundsDirectory() ) );

    QWidget* soundBrowseWidget = new QWidget();
    QHBoxLayout* soundBrowseLayout = new QHBoxLayout( soundBrowseWidget );
    soundBrowseLayout->addWidget( soundDirectoryLabel );
    soundBrowseLayout->addWidget( soundDirectoryEditor_ );
    soundBrowseLayout->addWidget( soundDirectoryButton );

    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( coordinateLabel );
    boxLayout->addWidget( CreateSlider( "directfire", tools::translate( "SoundPanel", "Direct fires" ), this ) );
    boxLayout->addWidget( CreateSlider( "indirectsmoke", tools::translate( "SoundPanel", "Fumigen fires" ), this ) );
    boxLayout->addWidget( CreateSlider( "indirectexplosive", tools::translate( "SoundPanel", "Explosive fires" ), this ) );
    boxLayout->addWidget( CreateSlider( "indirectillumination", tools::translate( "SoundPanel", "Illumination fires" ), this ) );
    boxLayout->addWidget( CreateSlider( "indirecteffect", tools::translate( "SoundPanel", "Neutralisation fires" ), this ) );
    boxLayout->addWidget( soundBrowseWidget );
    boxLayout->addStretch( 1 );
    connect( this, SIGNAL( soundSliderUpdated( std::string, int ) ), this, SLOT( OnChangeVolume( std::string, int ) ) );
    setWidget( box );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel destructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
SoundPanel::~SoundPanel()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnSliderChanged
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundPanel::OnSliderChanged( int value )
{
    QObject* slider = QObject::sender();
    QString name = slider->name();
    QStringList nameParts = name.split( "_" );
    emit soundSliderUpdated( nameParts[ nameParts.size() - 1 ].toStdString(), value );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnChangeVolume
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void SoundPanel::OnChangeVolume( const std::string& name, int value )
{
    SoundManager::GetInstance()->SetVolume( name, double( value )/ 10. );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnChooseSoundsDirectory
// Created: NPT 2013-07-08
// -----------------------------------------------------------------------------
void SoundPanel::OnChooseSoundsDirectory()
{
    tools::Path newDirectory = gui::FileDialog::getExistingDirectory( this, tr( "Select sounds directory" ), tools::Path::FromUnicode( soundDirectoryEditor_->text().toStdWString() ) );
    soundDirectoryEditor_->setText( QString::fromStdWString( newDirectory.ToUnicode() ) );
    SoundManager::GetInstance()->ChangeSoundsDirectory( soundDirectoryEditor_->text().toStdString().c_str() );
}

