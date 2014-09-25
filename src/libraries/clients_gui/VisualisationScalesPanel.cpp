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
#include "SubObjectName.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "ENT/ENT_Tr.h"

using namespace gui;

namespace
{
    const std::string strScale = "VisualisationScales/";
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel constructor
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
VisualisationScalesPanel::VisualisationScalesPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "VisualisationScalesPanel" )
    , controllers_( controllers )
{
    SubObjectName subObject( "VisualisationScalesPanel" );
    QWidget* container = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( container );
    RichGroupBox* box = new RichGroupBox( "visuScales", tr( "Visualisation scales" ) );
    QGridLayout* boxLayout = new QGridLayout( box );

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

    QLabel* minlabel = new QLabel( tr( "Min Scale" ), box );
    QLabel* maxLabel = new QLabel( tr( "Max Scale" ), box );
    boxLayout->addWidget( minlabel, 0, 1 );
    boxLayout->addWidget( maxLabel, 0, 2 );

    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        auto scale = static_cast< E_VisualisationScale >( i );
        QLabel* label = new QLabel( ENT_Tr::ConvertFromVisualisationScale( scale, ENT_Tr::eToTr ).c_str() );
        const std::string option = strScale + ENT_Tr::ConvertFromVisualisationScale( scale );
        currentScales_[ i ].min_ = controllers_.options_.GetOption( option + "/min" ).To< int >();
        currentScales_[ i ].max_ = controllers_.options_.GetOption( option + "/max" ).To< int >();
        minCombos_[ i ] = new RichWidget< QComboBox >( "minCombos" + QString::number( i ) );
        minCombos_[ i ]->insertStringList( scales );
        minCombos_[ i ]->setCurrentItem( ConvertFromScale( currentScales_[ i ].min_ ) );
        connect( minCombos_[ i ], SIGNAL( activated( int ) ), this, SLOT( OnValueChanged( int ) ) );
        maxCombos_[ i ] = new RichWidget< QComboBox >( "maxCombos" + QString::number( i ) );
        maxCombos_[ i ]->insertStringList( scales );
        maxCombos_[ i ]->setCurrentItem( ConvertFromScale( currentScales_[ i ].max_ ) );
        connect( maxCombos_[ i ], SIGNAL( activated( int ) ), this, SLOT( OnValueChanged( int ) ) );

        boxLayout->addWidget( label, i + 1, 0 );
        boxLayout->addWidget( minCombos_[ i ], i + 1, 1 );
        boxLayout->addWidget( maxCombos_[ i ], i + 1, 2 );
    }

    RichPushButton* button = new RichPushButton( "reset", tr( "Reset" ) );
    connect( button, SIGNAL( clicked() ), this, SLOT( OnReset() ) );
    boxLayout->addWidget( button, 15, 0 );
    box->setMinimumHeight( 500 );
    mainLayout->addWidget( box );
    mainLayout->addStretch( 1 );
    setWidget( container );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel destructor
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
VisualisationScalesPanel::~VisualisationScalesPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::Commit
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::Commit()
{
    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        const auto name = strScale + ENT_Tr::ConvertFromVisualisationScale( static_cast< E_VisualisationScale >( i ) );
        currentScales_[ i ].min_ = controllers_.options_.GetOption( name + "/min" ).To< int >();
        currentScales_[ i ].max_ = controllers_.options_.GetOption( name + "/max" ).To< int >();
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::Reset
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::Reset()
{
    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        const auto name = strScale + ENT_Tr::ConvertFromVisualisationScale( static_cast< E_VisualisationScale >( i ) );
        controllers_.options_.Change( name + "/min", currentScales_[ i ].min_ );
        controllers_.options_.Change( name + "/max", currentScales_[ i ].max_ );
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::OnValueChanged
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::OnValueChanged( int )
{
    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        if( minCombos_[ i ]->currentItem() > maxCombos_[ i ]->currentItem() )
            maxCombos_[ i ]->setCurrentItem( minCombos_[ i ]->currentItem() );
        const auto name = strScale + ENT_Tr::ConvertFromVisualisationScale( static_cast< E_VisualisationScale >( i ) );
        controllers_.options_.Change( name + "/min", ConvertToScale( minCombos_[ i ]->currentItem() ) );
        controllers_.options_.Change( name + "/max", ConvertToScale( maxCombos_[ i ]->currentItem() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::OnReset
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::OnReset()
{
    // TODO
/*    for( int i = 0; i < eNbrVisualisationScale; ++i )
    {
        const auto name = strScale + ENT_Tr::ConvertFromVisualisationScale( static_cast< E_VisualisationScale >( i ) );
        controllers_.options_.Change( name + "/min", DefaultScales[ i ].min_ );
        controllers_.options_.Change( name + "/max", DefaultScales[ i ].max_ );
    }
*/
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::OptionChanged
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::OptionChanged( const std::string& /*name*/, const kernel::OptionVariant& /*value*/ )
{
    // TODO
    /*if( name.size() > strMinScale.size() && name.substr( 0, strMinScale.size() ) == strMinScale )
    {
        std::stringstream stream( name.substr( strMinScale.size() ) );
        int index = -1;
        stream >> index;
        if( index >= 0 && index < eNbrVisualisationScale )
            minCombos_[ index ]->setCurrentItem( ConvertFromScale( value.To< int >() ) );
    }

    if( name.size() > strMaxScale.size() && name.substr( 0, strMaxScale.size() ) == strMaxScale )
    {
        std::stringstream stream( name.substr( strMaxScale.size() ) );
        int index = -1;
        stream >> index;
        if( index >= 0 && index < eNbrVisualisationScale )
            maxCombos_[ index ]->setCurrentItem( ConvertFromScale( value.To< int >() ) );
    }*/
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::ConvertFromScale
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
int VisualisationScalesPanel::ConvertFromScale( int scale )
{
    switch( scale )
    {
    case 0:
        return 0;
    case 2500:
        return 1;
    case 5000:
        return 2;
    case 10000:
        return 3;
    case 25000:
        return 4;
    case 50000:
        return 5;
    case 100000:
        return 6;
    case 250000:
        return 7;
    case 500000:
        return 8;
    case 1000000:
        return 9;
    case 2500000:
        return 10;
    case 5000000:
        return 11;
    default:
        return 12;
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::ConvertToScale
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
int VisualisationScalesPanel::ConvertToScale( int index )
{
    switch( index )
    {
    case 0:
        return 0;
    case 1:
        return 2500;
    case 2:
        return 5000;
    case 3:
        return 10000;
    case 4:
        return 25000;
    case 5:
        return 50000;
    case 6:
        return 100000;
    case 7:
        return 250000;
    case 8:
        return 500000;
    case 9:
        return 1000000;
    case 10:
        return 2500000;
    case 11:
        return 5000000;
    default:
        return 10000000;
    }
}
