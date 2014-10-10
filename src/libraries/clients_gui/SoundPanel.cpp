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
#include "OptionWidgets.h"
#include "RichGroupBox.h"
#include "RichLineEdit.h"
#include "RichPushButton.h"
#include "SoundManager.h"
#include "Tools.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include <boost/assign/list_of.hpp>
#include <boost/filesystem/operations.hpp>

using namespace gui;

namespace gui
{
    void AddOptionSlider( kernel::OptionsController& options,
                          QVBoxLayout* layout,
                          const QString& label,
                          const QString& objectName,
                          const std::string& optionName )
    {
        auto slider = new OptionSlider( options, objectName, optionName, 0, 100 );
        slider->setMaximumWidth( 200 );
        layout->addWidget( tools::AddLabeledWidget( label, slider ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundPanel constructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
SoundPanel::SoundPanel( QWidget* parent,
                        kernel::OptionsController& options,
                        SoundPlayer& soundPlayer )
    : PreferencePanel_ABC( parent, "SoundPanel" )
    , options_( options )
    , soundPlayer_( soundPlayer )
{
    RichGroupBox* box = new RichGroupBox( "soundControl", tr( "Ajust all sound volumes" ) );
    QVBoxLayout* boxLayout = new QVBoxLayout( box );

    AddOptionSlider( options, boxLayout, tr( "Direct fires" ), "directfire", "Sound/Volume/directfire" );
    AddOptionSlider( options, boxLayout, tr( "Fumigen fires" ), "indirectsmoke", "Sound/Volume/indirectsmoke" );
    AddOptionSlider( options, boxLayout, tr( "Explosive fires" ), "indirectexplosive", "Sound/Volume/indirectexplosive" );
    AddOptionSlider( options, boxLayout, tr( "Illumination fires" ), "indirectillumination", "Sound/Volume/indirectillumination" );
    AddOptionSlider( options, boxLayout, tr( "Neutralisation fires" ), "indirecteffect", "Sound/Volume/indirecteffect" );

    soundDirectoryEditor_ = new RichLineEdit( "soundDirectoryEditor" );
    QPushButton* soundDirectoryButton = new RichPushButton( "soundDirectoryButton", "..." );
    connect( soundDirectoryButton, SIGNAL( clicked( bool ) ), this, SLOT( OnChooseSoundsDirectory() ) );
    connect( soundDirectoryEditor_, SIGNAL( textChanged( const QString& ) ), this, SLOT( OnSoundsDirectoryChanged( const QString& ) ) );
    QWidget* soundBrowseWidget = new QWidget();
    QHBoxLayout* soundBrowseLayout = new QHBoxLayout( soundBrowseWidget );
    soundBrowseLayout->addWidget( new QLabel( tr( "Sound directory :" ) ) );
    soundBrowseLayout->addWidget( soundDirectoryEditor_ );
    soundBrowseLayout->addWidget( soundDirectoryButton );

    boxLayout->addWidget( soundBrowseWidget );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( new OptionCheckBox( options, "MuteSounds", "Sound/Mute", tr( "Mute sounds" ) ) );
    layout->addWidget( box );
    layout->addStretch( 1 );
    setLayout( layout );

    options_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel destructor
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
SoundPanel::~SoundPanel()
{
    options_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OptionChanged
// Created: NPT 2013-07-12
// -----------------------------------------------------------------------------
void SoundPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "Sound/Directory" )
    {
        const tools::Path directory = tools::Path::FromUnicode( value.To< QString >().toStdWString() ).Normalize();
        soundDirectoryEditor_->setText( QString::fromStdWString( directory.ToUnicode() ) );
    }
    else if( name == "Sound/Mute" )
    {
        soundPlayer_.SetSoundState( !value.To< bool >() );
    }
    else
    {
        QString soundName = name.c_str();
        if( !soundName.contains( "Sound/Volume/" ) )
            return;
        soundPlayer_.SetVolume( soundName.remove( "Sound/Volume/" ).toStdString(),
                                static_cast< double >( value.To< int >() ) / 100. );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::SetSoundDirectory
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
void SoundPanel::SetSoundDirectory( const tools::Path& path )
{
    if( !path.IsEmpty() && path.Exists() )
    {
        soundDirectoryEditor_->setText( QString::fromStdWString( path.ToUnicode() ) );
        soundPlayer_.ChangeSoundsDirectory( path );
    }
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnChooseSoundsDirectory
// Created: NPT 2013-07-08
// -----------------------------------------------------------------------------
void SoundPanel::OnChooseSoundsDirectory()
{
    const auto path = FileDialog::getExistingDirectory( this,
                                                        tr( "Select sounds directory" ),
                                                        tools::Path::FromUnicode( soundDirectoryEditor_->text().toStdWString() ) );
    SetSoundDirectory( path );
}

// -----------------------------------------------------------------------------
// Name: SoundPanel::OnSoundsDirectoryChanged
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
void SoundPanel::OnSoundsDirectoryChanged( const QString& text )
{
    SetSoundDirectory( tools::Path::FromUnicode( text.toStdWString() ) );
}
