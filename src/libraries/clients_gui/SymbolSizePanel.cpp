// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SymbolSizePanel.h"
#include "moc_SymbolSizePanel.cpp"
#include "GLView_ABC.h"
#include "OptionWidgets.h"
#include "SignalAdapter.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Tr.h"
#include <boost/algorithm/string.hpp>

using namespace gui;

namespace
{
    const std::string header = "SymbolSize/";
    const int maxValue = 8; // 8x
    const int minValue = -8; // 1/8
    const float ratio = 1.1f; // same percentage as in SymbolSizeOptionChooser
    const float fromRatio = std::log( ratio );

    float ToRatio( int value )
    {
        return std::pow( ratio, value );
    }

    int FromRatio( float y )
    {
        const float result = std::log( y ) / fromRatio;
        return static_cast< int >( result < 0 ? std::floor( result ) : std::ceil( result ) );
    }

    class NatureLevelSlider : public OptionWidget< RichWidget< QSlider > >
    {
    public:
        NatureLevelSlider( kernel::OptionsController& options,
                           const QString& objectName,
                           const std::string& optionName,
                           int min,
                           int max,
                           QWidget* parent = 0 )
            : OptionWidget< RichWidget< QSlider > >( options, objectName, optionName, parent )
        {
            setOrientation( Qt::Horizontal );
            setRange( min, max );
            gui::connect( this, SIGNAL( valueChanged( int ) ), [=,&options]{
                options.Change( optionName, ToRatio( value() ) );
            } );
        }

    private:
        virtual void OnOptionChanged( const kernel::OptionVariant& value )
        {
            setValue( FromRatio( value.To< float >() ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: SymbolSizePanel constructor
// Created: SLI 2014-06-18
// -----------------------------------------------------------------------------
SymbolSizePanel::SymbolSizePanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "SymbolSizePanel" )
{
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, tr( "Hierarchical levels relative sizes" ), this );
    for( unsigned int i = eNatureLevel_xxxxx; i > eNatureLevel_None; --i )
    {
        new QLabel( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( i ), ENT_Tr::eToTr ).c_str(), box );
        const std::string& level = ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( i ) );
        new NatureLevelSlider( options, ( "SymbolSizePanel" + level + "Slider" ).c_str(), header + level, minValue, maxValue, box );
    }
    new QLabel( tr( "Headquarters" ), box );
    new NatureLevelSlider( options, "SymbolSizePanelHeadQuartersSlider", header + "Headquarters", minValue, maxValue, box );
    setWidget( box );
}

// -----------------------------------------------------------------------------
// Name: SymbolSizePanel destructor
// Created: SLI 2014-06-18
// -----------------------------------------------------------------------------
SymbolSizePanel::~SymbolSizePanel()
{
    // NOTHING
}
