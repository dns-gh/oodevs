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
#include "CheckBox.h"
#include "FileDialog.h"
#include "RichGroupBox.h"
#include "RichLineEdit.h"
#include "RichPushButton.h"
#include "RichSlider.h"
#include "SoundManager.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"

#include "tools/GeneralConfig.h"

#include <boost/assign/list_of.hpp>
#include <boost/filesystem/operations.hpp>

using namespace gui;

namespace
{
    QWidget* CreateSlider( const QString& name, const QString& labelName, SoundPanel* parent, int value )
    {
        QLabel* label = new QLabel( labelName );

        RichSlider* slider = new RichSlider( name );
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

    // TODO à passer dans ENT (attention, c'est hardcodé à plein d'endroits)
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
SoundPanel::SoundPanel( QWidget* parent, kernel::Controllers& controllers, SoundPlayer& soundPlayer )
    : PreferencePanel_ABC( parent, "SoundPanel" )
    , controllers_( controllers )
    , soundPlayer_( soundPlayer )
{
    RichGroupBox* box = new RichGroupBox( "soundControl", tools::translate( "SoundPanel", "Sound control" ), this );

    QLabel* soundDirectoryLabel = new QLabel( tools::translate( "SoundPanel", "Sound directory :" ) );
    const std::string soundPath = controllers_.options_.GetOption( "Sound/Directory" ).To< QString >().toStdString();
    soundDirectory_ = soundPath.c_str();
    soundDirectoryEditor_ = new RichLineEdit( "soundDirectoryEditor", soundPath.c_str() );
    QPushButton* soundDirectoryButton = new RichPushButton( "soundDirectoryButton", "..." );
    connect( soundDirectoryButton, SIGNAL( clicked( bool ) ), this, SLOT( OnChooseSoundsDirectory() ) );

    QWidget* soundBrowseWidget = new QWidget();
    QHBoxLayout* soundBrowseLayout = new QHBoxLayout( soundBrowseWidget );
    soundBrowseLayout->addWidget( soundDirectoryLabel );
    soundBrowseLayout->addWidget( soundDirectoryEditor_ );
    soundBrowseLayout->addWidget( soundDirectoryButton );

    for( auto it = ::soundList.begin(); it != ::soundList.end(); ++it )
        soundValues_[ *it ] = controllers_.options_.GetOption( "Sound/Volume/" + *it ).To< int >();

    activated_ = new CheckBox( "MuteSounds", tools::translate( "SoundPanel", "Mute sounds" ) );
    connect( activated_, SIGNAL( toggled( bool ) ), SLOT( OnStateChanged( bool ) ) );

    RichGroupBox* coordinateBox = new RichGroupBox( "soundControl", tools::translate( "SoundPanel", "Ajust all sound volumes" ), box );
    QVBoxLayout* subBoxLayout = new QVBoxLayout( coordinateBox );
    subBoxLayout->addWidget( CreateSlider( "directfire", tools::translate( "SoundPanel", "Direct fires" ), this, soundValues_[ "directfire" ] ) );
    subBoxLayout->addWidget( CreateSlider( "indirectsmoke", tools::translate( "SoundPanel", "Fumigen fires" ), this, soundValues_[ "indirectsmoke" ] ) );
    subBoxLayout->addWidget( CreateSlider( "indirectexplosive", tools::translate( "SoundPanel", "Explosive fires" ), this, soundValues_[ "indirectexplosive" ] ) );
    subBoxLayout->addWidget( CreateSlider( "indirectillumination", tools::translate( "SoundPanel", "Illumination fires" ), this, soundValues_[ "indirectillumination" ] ) );
    subBoxLayout->addWidget( CreateSlider( "indirecteffect", tools::translate( "SoundPanel", "Neutralisation fires" ), this, soundValues_[ "indirecteffect" ] ) );
    subBoxLayout->addWidget( soundBrowseWidget );
    subBoxLayout->addStretch( 1 );

    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->addWidget( activated_ );
    boxLayout->addWidget( coordinateBox );
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
        controllers_.options_.Change( "Sound/Volume/" + *it, soundValues_[ *it ] );
        if( soundSliders_[ *it ] )
            soundSliders_[ *it ]->setValue( soundValues_[ *it ] );
    }
    controllers_.options_.Change( "Sound/Directory", QString( soundDirectory_.Normalize().ToUTF8().c_str() ) );
    soundDirectoryEditor_->setText( soundDirectory_.Normalize().ToUTF8().c_str() );
    activated_->Revert();
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::Commit
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
void SoundPanel::Commit()
{
    for( auto it = ::soundList.begin(); it != ::soundList.end(); ++it )
    {
        controllers_.options_.Change( "Sound/Volume/" + *it, soundSliders_[ *it ]->value() );
        OnChangeVolume( *it, soundSliders_[ *it ]->value() );
    }
    controllers_.options_.Change( "Sound/Directory", soundDirectoryEditor_->text() );
    activated_->Commit();
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OptionChanged
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
void SoundPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "Sound/Directory" )
    {
        soundDirectory_ = value.To< QString >().toStdString().c_str();
        soundDirectoryEditor_->setText( soundDirectory_.Normalize().ToUTF8().c_str() );
    }
    else
    {
        QString soundName = name.c_str();
        if( !soundName.contains( "Sound/Volume/" ) )
            return;
        for( auto it = soundValues_.begin(); it != soundValues_.end(); ++it )
        {
            if( it->first == soundName.remove( "Sound/Volume/" ).toStdString() )
            {
                it->second = value.To< int >();
                soundSliders_[ it->first ]->setValue( it->second );
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
    soundPlayer_.SetVolume( name, double( value )/ 100. );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnChooseSoundsDirectory
// Created: NPT 2013-07-08
// -----------------------------------------------------------------------------
void SoundPanel::OnChooseSoundsDirectory()
{
    tools::Path newDirectory = FileDialog::getExistingDirectory( this, tr( "Select sounds directory" ), tools::Path::FromUnicode( soundDirectoryEditor_->text().toStdWString() ) );
    if( !newDirectory.IsEmpty() )
    {
        soundDirectoryEditor_->setText( QString::fromStdWString( newDirectory.ToUnicode() ) );
        soundPlayer_.ChangeSoundsDirectory( soundDirectoryEditor_->text().toStdString().c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::GetSoundSliders
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
std::map< std::string, RichSlider*>& SoundPanel::GetSoundSliders()
{
    return soundSliders_;
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnStateChanged
// Created: LGY 2013-08-27
// -----------------------------------------------------------------------------
void SoundPanel::OnStateChanged( bool state )
{
    soundPlayer_.SetSoundState( !state );
}
