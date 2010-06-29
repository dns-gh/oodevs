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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

/* TRANSLATOR gui::VisualisationScalesPanel */

using namespace gui;

const std::string VisualisationScalesPanel::strMinScale( "VisuScaleMin" );
const std::string VisualisationScalesPanel::strMaxScale( "VisuScaleMax" );

const VisualisationScalesPanel::Scale VisualisationScalesPanel::DefaultScales[ 14 ] =
    { { 10, 50 },     { 0, 10 },  { 0, 50 }, { 0, 10 },  { 5, 1000000 }
    , { 5, 250 },     { 5, 25 },  { 0, 5 },  { 5, 250 }, { 0, 25 }
    , { 5, 1000000 }, { 5, 250 }, { 0, 25 }, { 0, 10 } };

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel constructor
// Created: JSR 2010-06-14
// -----------------------------------------------------------------------------
VisualisationScalesPanel::VisualisationScalesPanel( QWidget* parent, kernel::Controllers& controllers )
    : PreferencePanel_ABC( parent, "VisualisationScalesPanel" )
    , controllers_( controllers )
{
    QGroupBox* box = new QGroupBox( 3, Qt::Horizontal, tr( "Visualisation scales" ), this );

    const QString elements[ 14 ] =
    { tr( "Large texts" ), tr( "Small texts" ), tr( "Edges" ),           tr( "Cliffs" ),
      tr( "Highways" ),    tr( "Main roads" ),  tr( "Secondary roads" ), tr( "Country roads" ),
      tr( "Bridges" ),     tr( "Railroads" ),   tr( "Main rivers" ),     tr( "Rivers" ),
      tr( "Streams" ),     tr( "Urban blocks" ) };

    QStringList scales;
    scales.push_back( "1:1" );
    scales.push_back( "1:2.5" );
    scales.push_back( "1:5" );
    scales.push_back( "1:10" );
    scales.push_back( "1:25" );
    scales.push_back( "1:50" );
    scales.push_back( "1:100" );
    scales.push_back( "1:250" );
    scales.push_back( "1:500" );
    scales.push_back( "1:1000" );
    scales.push_back( "1:2500" );
    scales.push_back( "1:5000" );
    scales.push_back( "1:10000" );

    new QWidget( box );
    new QLabel( tr( "Min Scale" ), box );
    new QLabel( tr( "Max Scale" ), box );
    for( int i = 0; i < 14; ++i )
    {
        new QLabel( elements[ i ], box );
        std::stringstream stream;
        stream << i;
        currentScales_[ i ].min_ = controllers_.options_.GetOption( strMinScale + stream.str(), DefaultScales[ i ].min_ ).To< int >();
        currentScales_[ i ].max_ = controllers_.options_.GetOption( strMaxScale + stream.str(), DefaultScales[ i ].max_ ).To< int >();
        minCombos_[ i ] = new QComboBox( box );
        minCombos_[ i ]->insertStringList( scales );
        minCombos_[ i ]->setCurrentItem( ConvertFromScale( currentScales_[ i ].min_ ) );
        connect( minCombos_[ i ], SIGNAL( activated( int ) ), this, SLOT( OnValueChanged( int ) ) );
        maxCombos_[ i ] = new QComboBox( box );
        maxCombos_[ i ]->insertStringList( scales );
        maxCombos_[ i ]->setCurrentItem( ConvertFromScale( currentScales_[ i ].max_ ) );
        connect( maxCombos_[ i ], SIGNAL( activated( int ) ), this, SLOT( OnValueChanged( int ) ) );
    }

    QPushButton* button = new QPushButton( tr( "Reset" ), box );
    connect( button, SIGNAL( clicked() ), this, SLOT( OnReset() ) );
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
    for( int i = 0; i < 14; ++i )
    {
        std::stringstream stream;
        stream << i;
        currentScales_[ i ].min_ = controllers_.options_.GetOption( strMinScale + stream.str(), currentScales_[ i ].min_ ).To< int >();
        currentScales_[ i ].max_ = controllers_.options_.GetOption( strMaxScale + stream.str(), currentScales_[ i ].max_ ).To< int >();
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::Reset
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::Reset()
{
    for( int i = 0; i < 14; ++i )
    {
        std::stringstream stream;
        stream << i;
        controllers_.options_.Change( strMinScale + stream.str(), currentScales_[ i ].min_ );
        controllers_.options_.Change( strMaxScale + stream.str(), currentScales_[ i ].max_ );
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::OnValueChanged
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::OnValueChanged( int )
{
    for( int i = 0; i < 14; ++i )
    {
        if( minCombos_[ i ]->currentItem() > maxCombos_[ i ]->currentItem() )
            maxCombos_[ i ]->setCurrentItem( minCombos_[ i ]->currentItem() );

        std::stringstream stream;
        stream << i;
        controllers_.options_.Change( strMinScale + stream.str(), ConvertToScale( minCombos_[ i ]->currentItem() ) );
        controllers_.options_.Change( strMaxScale + stream.str(), ConvertToScale( maxCombos_[ i ]->currentItem() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::OnReset
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::OnReset()
{
    for( int i = 0; i < 14; ++i )
    {
        std::stringstream stream;
        stream << i;
        controllers_.options_.Change( strMinScale + stream.str(), DefaultScales[ i ].min_ );
        controllers_.options_.Change( strMaxScale + stream.str(), DefaultScales[ i ].max_ );
    }
}

// -----------------------------------------------------------------------------
// Name: VisualisationScalesPanel::OptionChanged
// Created: JSR 2010-06-15
// -----------------------------------------------------------------------------
void VisualisationScalesPanel::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name.size() > strMinScale.size() && name.substr( 0, strMinScale.size() ) == strMinScale )
    {
        std::stringstream stream( name.substr( strMinScale.size() ) );
        int index = -1;
        stream >> index;
        if( index >= 0 && index < 14 )
            minCombos_[ index ]->setCurrentItem( ConvertFromScale( value.To< int >() ) );
    }

    if( name.size() > strMaxScale.size() && name.substr( 0, strMaxScale.size() ) == strMaxScale )
    {
        std::stringstream stream( name.substr( strMaxScale.size() ) );
        int index = -1;
        stream >> index;
        if( index >= 0 && index < 14 )
            maxCombos_[ index ]->setCurrentItem( ConvertFromScale( value.To< int >() ) );
    }
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
    case 2:
        return 1;
    case 5:
        return 2;
    case 10:
        return 3;
    case 25:
        return 4;
    case 50:
        return 5;
    case 100:
        return 6;
    case 250:
        return 7;
    case 500:
        return 8;
    case 1000:
        return 9;
    case 2500:
        return 10;
    case 5000:
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
        return 2;
    case 2:
        return 5;
    case 3:
        return 10;
    case 4:
        return 25;
    case 5:
        return 50;
    case 6:
        return 100;
    case 7:
        return 250;
    case 8:
        return 500;
    case 9:
        return 1000;
    case 10:
        return 2500;
    case 11:
        return 5000;
    default:
        return 1000000;
    }
}
