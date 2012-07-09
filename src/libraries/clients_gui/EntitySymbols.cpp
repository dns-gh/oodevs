// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntitySymbols.h"
#include "SymbolIcons.h"
#include "ColorStrategy_ABC.h"
#include "IconsRenderPass.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntitySymbols::EntitySymbols
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
EntitySymbols::EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy )
    : icons_   ( icons )
    , strategy_( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::~EntitySymbols
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
EntitySymbols::~EntitySymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !symbol )
        return icons_.GetDefaultSymbol();
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return icons_.GetDefaultSymbol();
    return GetSymbol( entity, symbolName, levelName, size );
}

namespace
{
    // $$$$ LGY 2012-02-07 : hardcoded for displaying !!!
    bool ShouldPaintPixelAtCoordinateForKarma( const kernel::Karma& karma, int x, int y )
    {
        if( karma == kernel::Karma::friend_ )
            return ( x == 0 || x == 1 ) && y > 11;
        else if( karma == kernel::Karma::enemy_ )
            return ( x == 0 || x == 1 ) && y > 21;
        else if( karma == kernel::Karma::neutral_ )
            return ( x == 3 || x == 4 ) && y > 10;
        return ( x == 0 || x == 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: LGY 2011-07-22
// -----------------------------------------------------------------------------
const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const std::string& symbolName, const std::string& levelName,
                                         const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    SymbolIcon icon( symbolName, levelName );
    icon.SetColor( strategy_.FindColor( entity ) );
    icon.SetSize( size );
    if( const kernel::TacticalHierarchies* pHierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( const kernel::Diplomacies_ABC* pDiplomacy = pHierarchy->GetTop().Retrieve< kernel::Diplomacies_ABC >() )
            icon.SetKarmaFactor( pDiplomacy->GetKarma() );
    return icons_.GetSymbol( icon );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: LGY 2012-02-06
// -----------------------------------------------------------------------------
QPixmap EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const QPixmap& pixmap ) const
{
    if( !IsCommandPost( entity ) )
        return pixmap;

    if( const kernel::TacticalHierarchies* pHierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
        if( const kernel::Diplomacies_ABC* pDiplomacy = pHierarchy->GetTop().Retrieve< kernel::Diplomacies_ABC >() )
        {
            const kernel::Karma& karma= pDiplomacy->GetKarma();
            QImage symbol = pixmap.toImage();
            unsigned int height = static_cast< unsigned int >( 32 * RENDER_FACTOR ) + headquarterSize_;
            QImage headquarter( 32, height, QImage::Format_ARGB32 );
            memcpy( headquarter.bits(), symbol.bits(), symbol.byteCount() );
            uchar* ptr = headquarter.bits();
            const int blue = 0, green = 1, red = 2, alpha = 3;
            for( unsigned int y = height - headquarterSize_; y < height; ++y )
                for( unsigned int x = 0; x < 32; ++x )
                {
                    int offset = 4 * x + y * headquarter.bytesPerLine() ;
                    ptr[ offset + blue  ] = 0x00;
                    ptr[ offset + green ] = 0x00;
                    ptr[ offset + red   ] = 0x00;
                    ptr[ offset + alpha ] = 0x00;
                }
            for( unsigned int y = 0; y < height; ++y )
                for( unsigned int x = 0; x < 32; ++x )
                {
                    int offset = 4 * x + y * headquarter.bytesPerLine() ;
                    if( ShouldPaintPixelAtCoordinateForKarma( karma, x, y ) )
                    {
                        ptr[ offset + blue  ] = 0x00;
                        ptr[ offset + green ] = 0x00;
                        ptr[ offset + red   ] = 0x00;
                        ptr[ offset + alpha ] = 0xff;
                    }
                }
            return QPixmap( headquarter );
        }
    return pixmap;
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::IsCommandPost
// Created: LGY 2012-02-06
// -----------------------------------------------------------------------------
bool EntitySymbols::IsCommandPost( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::CommandPostAttributes_ABC* pAttributes = entity.Retrieve< kernel::CommandPostAttributes_ABC >() )
        return pAttributes->IsCommandPost();
    return false;
}
