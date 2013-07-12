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
#include "RichGroupBox.h"
#include "RichLineEdit.h"
#include "RichPushButton.h"
#include "RichSlider.h"
#include "SoundManager.h"
#include "Tools.h"
#include "tools/GeneralConfig.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/operations.hpp>

using namespace gui;

namespace
{
    QWidget* CreateSlider( const QString& name, const QString& labelName, SoundPanel* parent, int value )
    {
        QLabel* label = new QLabel( labelName );

        gui::RichSlider* slider = new RichSlider( name );
        slider->setRange( 0, 100 );
        slider->setOrientation( Qt::Horizontal );
        slider->setMaximumWidth( 200 );
        slider->setValue( value );
        parent->connect( slider, SIGNAL( valueChanged( int ) ), parent, SLOT( OnSliderChanged( int ) ) );

        QWidget* sliderBox = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout( sliderBox );
        layout->addWidget( label, 1 );
        layout->addWidget( slider, 1 );
        parent->GetSoundSliders()[ name.toStdString() ] = slider;
        return sliderBox;
    }

    const std::vector< std::string > soundList = boost::assign::list_of< std::string >
        ( "directfire" )
        ( "indirectsmoke" )
        ( "indirectexplosive" )
        ( "indirectillumination" )
        ( "indirecteffect" );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel constructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
SoundPanel::SoundPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "SoundPanel" )
    , controllers_( controllers )
{
    QLabel* coordinateLabel = new QLabel( tools::translate( "SoundPanel", "Ajust all sound volumes:" ) );
    RichGroupBox* box = new RichGroupBox( "soundControl", tools::translate( "SoundPanel", "Sound control" ), this );

    QLabel* soundDirectoryLabel = new QLabel( tools::translate( "SoundPanel", "Sound directory :" ) );
    tools::Path soundPath;
    soundPath = soundPath.Absolute( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) );

    soundDirectory_ = controllers_.options_.GetOption( "soundDirectory", soundPath.Normalize().ToUTF8() ).To< std::string >().c_str();
    soundDirectoryEditor_ = new gui::RichLineEdit( "soundDirectoryEditor", soundPath.Normalize().ToUTF8().c_str() );
    QPushButton* soundDirectoryButton = new gui::RichPushButton( "soundDirectoryButton", tools::translate( "SoundPanel", "..." ) );
    connect( soundDirectoryButton, SIGNAL( clicked( bool ) ), this, SLOT( OnChooseSoundsDirectory() ) );

    QWidget* soundBrowseWidget = new QWidget();
    QHBoxLayout* soundBrowseLayout = new QHBoxLayout( soundBrowseWidget );
    soundBrowseLayout->addWidget( soundDirectoryLabel );
    soundBrowseLayout->addWidget( soundDirectoryEditor_ );
    soundBrowseLayout->addWidget( soundDirectoryButton );

    for( auto it = ::soundList.begin(); it != ::soundList.end(); ++it )
        soundValues_[ *it ] = controllers_.options_.GetOption( *it + "volume", 50 ).To< int >();

    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( coordinateLabel );
    boxLayout->addWidget( CreateSlider( "directfire", tools::translate( "SoundPanel", "Direct fires" ), this, soundValues_[ "directfire" ] ) );
    boxLayout->addWidget( CreateSlider( "indirectsmoke", tools::translate( "SoundPanel", "Fumigen fires" ), this, soundValues_[ "indirectsmoke" ] ) );
    boxLayout->addWidget( CreateSlider( "indirectexplosive", tools::translate( "SoundPanel", "Explosive fires" ), this, soundValues_[ "indirectexplosive" ] ) );
    boxLayout->addWidget( CreateSlider( "indirectillumination", tools::translate( "SoundPanel", "Illumination fires" ), this, soundValues_[ "indirectillumination" ] ) );
    boxLayout->addWidget( CreateSlider( "indirecteffect", tools::translate( "SoundPanel", "Neutralisation fires" ), this, soundValues_[ "indirecteffect" ] ) );
    boxLayout->addWidget( soundBrowseWidget );
    boxLayout->addStretch( 1 );
    connect( this, SIGNAL( soundSliderUpdated( std::string, int ) ), this, SLOT( OnChangeVolume( std::string, int ) ) );
    setWidget( box );
    controllers_.Register( *this );
    Commit();
}

// -----------------------------------------------------------------------------
// Name: SoundPanel destructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
SoundPanel::~SoundPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::Reset
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
void SoundPanel::Reset()
{
    for( auto it = ::soundList.begin(); it != ::soundList.end(); ++it )
    {        
        controllers_.options_.Change( *it + "volume", soundValues_[ *it ] );
        if( soundSliders_[ *it ] )
            soundSliders_[ *it ]->setValue( soundValues_[ *it ] );
    }
    controllers_.options_.Change( "soundDirectory", soundDirectory_.Normalize().ToUTF8() );
    soundDirectoryEditor_->setText( soundDirectory_.Normalize().ToUTF8().c_str() );
}
// -----------------------------------------------------------------------------
// Name: SoundPanel::Commit
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
void SoundPanel::Commit()
{
    for( auto it = ::soundList.begin(); it != ::soundList.end(); ++it )
    {
        controllers_.options_.Change( *it + "volume", soundSliders_[ *it ]->value() );
        OnChangeVolume( *it, soundSliders_[ *it ]->value() );
    }
    controllers_.options_.Change( "soundDirectory", soundDirectoryEditor_->text().toStdString() );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OptionChanged
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
void SoundPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "soundDirectory" )
        soundDirectory_ = value.To< std::string >().c_str();
    else
    {
        QString soundName = name.c_str();
        if( !soundName.contains( "volume" ) )
            return;
        for( auto it = soundValues_.begin(); it != soundValues_.end(); ++it )
        {
            if( it->first == soundName.remove( "volume" ).toStdString() )
            {
                it->second = value.To< int >();
                soundSliders_[ soundName.remove( "volume" ).toStdString() ]->setValue( it->second );
                return;
            }
        }
    }
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
    SoundManager::GetInstance()->SetVolume( name, double( value )/ 100. );
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

// -----------------------------------------------------------------------------
// Name: std::map< std::string, RichSlider*> SoundPanel::GetSoundSliders
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
std::map< std::string, RichSlider*>& SoundPanel::GetSoundSliders()
{
    return soundSliders_;
}

