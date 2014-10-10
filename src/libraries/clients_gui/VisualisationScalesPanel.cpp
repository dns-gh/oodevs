// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "VisualisationScalesPanel.h"
#include "moc_VisualisationScalesPanel.cpp"
#include "RichPushButton.h"
#include "RichWidget.h"
#include "RichGroupBox.h"
#include "SignalAdapter.h"
#include "SubObjectName.h"
#include "clients_kernel/DefaultVisualisationScales.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

namespace
{
    const std::string strScale = "VisualisationScales/";
    int ConvertFromScale( int scale )
    {
        switch( scale )
        {
        case 0:       return 0;
        case 2500:    return 1;
        case 5000:    return 2;
        case 10000:   return 3;
        case 25000:   return 4;
        case 50000:   return 5;
        case 100000:  return 6;
        case 250000:  return 7;
        case 500000:  return 8;
        case 1000000: return 9;
        case 2500000: return 10;
        case 5000000: return 11;
        default:      return 12;
        }
    }
    int ConvertToScale( int index )
    {
        switch( index )
        {
        case 0:  return 0;
        case 1:  return 2500;
        case 2:  return 5000;
        case 3:  return 10000;
        case 4:  return 25000;
        case 5:  return 50000;
        case 6:  return 100000;
        case 7:  return 250000;
        case 8:  return 500000;
        case 9:  return 1000000;
        case 10: return 2500000;
        case 11: return 5000000;
        default: return 10000000;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel constructor
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
VisualisationScalesPanel::VisualisationScalesPanel( QWidget* parent, kernel::OptionsController& options )
    : PreferencePanel_ABC( parent, "VisualisationScalesPanel" )
    , options_( options )
{
    SubObjectName subObject( "VisualisationScalesPanel" );

    QStringList scales;
    scales.push_back( "1:100" );
    scales.push_back( "1:2500" );
    scales.push_back( "1:5000" );
    scales.push_back( "1:10000" );
    scales.push_back( "1:25000" );
    scales.push_back( "1:50000" );
    scales.push_back( "1:100000" );
    scales.push_back( "1:250000" );
    scales.push_back( "1:500000" );
    scales.push_back( "1:1000000" );
    scales.push_back( "1:2500000" );
    scales.push_back( "1:5000000" );
    scales.push_back( "1:10000000" );

    QGridLayout* visualisationLayout = new QGridLayout();
    visualisationLayout->addWidget( new QLabel( tr( "Min Scale" ) ), 0, 1 );
    visualisationLayout->addWidget( new QLabel( tr( "Max Scale" ) ), 0, 2 );

    const auto onValueChanged = [&](){
        for( int i = 0; i < eNbrVisualisationScale; ++i )
        {
            const auto name = strScale + ENT_Tr::ConvertFromVisualisationScale( static_cast< E_VisualisationScale >( i ) );
            if( minCombos_[ i ]->currentItem() > maxCombos_[ i ]->currentItem() )
                maxCombos_[ i ]->setCurrentItem( minCombos_[ i ]->currentItem() );
            options_.Change( name + "/Min", ConvertToScale( minCombos_[ i ]->currentItem() ) );
            options_.Change( name + "/Max", ConvertToScale( maxCombos_[ i ]->currentItem() ) );
        }
    };
    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        const auto scale = static_cast< E_VisualisationScale >( i );
        QLabel* label = new QLabel( ENT_Tr::ConvertFromVisualisationScale( scale, ENT_Tr::eToTr ).c_str() );
        const std::string option = strScale + ENT_Tr::ConvertFromVisualisationScale( scale );
        currentScales_[ i ].min_ = options_.GetOption( option + "/Min" ).To< int >();
        currentScales_[ i ].max_ = options_.GetOption( option + "/Max" ).To< int >();
        minCombos_[ i ] = new RichWidget< QComboBox >( "minCombos" + QString::number( i ) );
        minCombos_[ i ]->insertStringList( scales );
        minCombos_[ i ]->setCurrentItem( ConvertFromScale( currentScales_[ i ].min_ ) );
        gui::connect( minCombos_[ i ], SIGNAL( activated( int ) ), onValueChanged );
        maxCombos_[ i ] = new RichWidget< QComboBox >( "maxCombos" + QString::number( i ) );
        maxCombos_[ i ]->insertStringList( scales );
        maxCombos_[ i ]->setCurrentItem( ConvertFromScale( currentScales_[ i ].max_ ) );
        gui::connect( maxCombos_[ i ], SIGNAL( activated( int ) ), onValueChanged );

        visualisationLayout->addWidget( label, i + 1, 0 );
        visualisationLayout->addWidget( minCombos_[ i ], i + 1, 1 );
        visualisationLayout->addWidget( maxCombos_[ i ], i + 1, 2 );
    }
    RichPushButton* button = new RichPushButton( "reset", tr( "Reset" ) );
    gui::connect( button, SIGNAL( clicked() ), [&](){
        for( int i = 0; i < kernel::DefaultVisualisationScales::size_; ++i )
        {
            const auto& scale = kernel::DefaultVisualisationScales::data_[i];
            const auto name = strScale + ENT_Tr::ConvertFromVisualisationScale( scale.type_ );
            options_.Change( name + "/Min", scale.min_ );
            options_.Change( name + "/Max", scale.max_ );
            minCombos_[ i ]->blockSignals( true );
            maxCombos_[ i ]->blockSignals( true );
            minCombos_[ i ]->setCurrentItem( ConvertFromScale( scale.min_ ) );
            maxCombos_[ i ]->setCurrentItem( ConvertFromScale( scale.max_ ) );
            minCombos_[ i ]->blockSignals( false );
            maxCombos_[ i ]->blockSignals( false );
        }
    } );
    visualisationLayout->addWidget( button, 15, 0 );

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addLayout( visualisationLayout );
    layout->addStretch( 1 );
    setLayout( layout );
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel destructor
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
VisualisationScalesPanel::~VisualisationScalesPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::Load
// Created: ABR 2014-10-01
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::Load( const GlProxy& )
{
    options_.GetViewOptions()->Apply( [&]( const std::string& name, const kernel::OptionVariant& value, bool ) {
        if( name.find( strScale ) != 0 )
            return;
        const size_t baseSize = strScale.size();
        const size_t separator = name.find_last_of( "/" );
        if( separator == std::string::npos || separator <= baseSize )
            return;
        const auto scale = name.substr( baseSize, separator - baseSize );
        const auto option = name.substr( separator );
        const auto scaleType = ENT_Tr::ConvertToVisualisationScale( scale );
        if( option == "/Min" )
            minCombos_[ scaleType ]->setCurrentItem( ConvertFromScale( value.To< int >() ) );
        else if( option == "/Max" )
            maxCombos_[ scaleType ]->setCurrentItem( ConvertFromScale( value.To< int >() ) );
    } );
}
