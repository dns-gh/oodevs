// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainSettings.h"
#include "TerrainSetting.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include <terrain/GraphicData.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainSettings constructor
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
TerrainSettings::TerrainSettings()
    : alpha_( 1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings destructor
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
TerrainSettings::~TerrainSettings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::SetAlpha
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
void TerrainSettings::SetAlpha( float alpha )
{
    alpha_ = alpha;
}

namespace
{
    void SetColor( const TerrainSetting& setting, float alpha )
    {
        glColor4f( setting.color_.red() / 255.f,
                   setting.color_.green() / 255.f,
                   setting.color_.blue() / 255.f,
                   setting.shown_ ? alpha : 0.f );
    }
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::SetupLineGraphics
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
void TerrainSettings::SetupLineGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const auto& setting = settings_[ data ];
    if( !setting )
        return;
    glLineWidth( setting->lineWidth_ );
    SetColor( *setting, alpha_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::SetupLineGraphics
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
void TerrainSettings::SetupLineGraphics( unsigned int /* offset */ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::SetupBorderGraphics
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
void TerrainSettings::SetupBorderGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const auto& setting = settings_[ data ];
    if( !setting )
        return;
    glLineWidth( setting->lineWidth_ );
    SetColor( *setting, alpha_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::SetupAreaGraphics
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
void TerrainSettings::SetupAreaGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const auto& setting = settings_[ data ];
    if( !setting )
        return;
    SetColor( *setting, alpha_ * 0.5f );
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::Apply
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
void TerrainSettings::Apply( const std::function< void( const std::shared_ptr< TerrainSetting >& ) >& functor ) const
{
    for( auto it = settings_.begin(); it != settings_.end(); ++it )
        functor( it->second );
}

// -----------------------------------------------------------------------------
// Name: TerrainSettings::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void TerrainSettings::Load( const kernel::Options& options )
{
    settings_.clear();
    options.Apply( [&]( const std::string& name, const kernel::OptionVariant&, bool ) { 
        if( name.find( "Terrains/" ) != 0 )
            return;
        QStringList names = QString( name.c_str() ).split( "/" );
        if( names.size() != 3 )
            return;
        const auto type = names[ 1 ].toStdString();
        bool exist = false;
        for( auto it = settings_.begin(); it != settings_.end(); ++it )
            exist |= it->second->type_ == type;
        if( !exist )
            settings_[ TerrainData( type ) ] = std::make_shared< TerrainSetting >( type );
    } );
    for( auto it = settings_.begin(); it != settings_.end(); ++it )
    {
        auto& setting = it->second;
        const std::string root = "Terrains/" + setting->type_ + "/";
        setting->name_ = options.Get( root + "Name" ).To< QString >();
        setting->category_ = options.Get( root + "Category" ).To< QString >();
        setting->lineWidth_ = options.Get( root + "Width" ).To< float >();
        setting->color_ = QColor( options.Get( root + "Color" ).To< QString >() );
        setting->shown_ = options.Get( root + "Shown" ).To< bool >();
    }
}
