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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
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
    return icons_.GetSymbol( icon );
}

namespace
{
    bool IsValid( const kernel::Karma& karma, int x, int y )
    {
        switch( karma.GetUId() )
        {
        case 1:
            return ( x == 0 || x == 1 ) && y > 5;
        case 2:
            return ( x == 0 || x == 1 ) && y > 14;
        case 3:
            return ( x == 3 || x == 4 ) && y > 4;
        default:
            return ( x == 0 || x == 1 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: LGY 2012-02-06
// -----------------------------------------------------------------------------
QPixmap EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const QPixmap& pixmap ) const
{
    if( !IsCommandPost( entity ) )
        return pixmap;

    if( const kernel::CommunicationHierarchies* pHierarchy = entity.Retrieve< kernel::CommunicationHierarchies >() )
        if( const kernel::Diplomacies_ABC* pDiplomacy = pHierarchy->GetTop().Retrieve< kernel::Diplomacies_ABC >() )
        {
            const kernel::Karma& karma= pDiplomacy->GetKarma();
            QImage symbol = pixmap.toImage();
            QImage headquarter( 32, 48, QImage::Format_ARGB32 );
            memcpy( headquarter.bits(), symbol.bits(), symbol.byteCount() );
            uchar* ptr = headquarter.bits();
            for( int y = 0; y < 48; ++y )
                for( int x = 0; x < 32; ++x )
                {
                    if( y > 31 )
                    {
                        ptr[ 4 * x + y * headquarter.bytesPerLine() ]     = 0xff;
                        ptr[ 4 * x + y * headquarter.bytesPerLine() + 1 ] = 0xff;
                        ptr[ 4 * x + y * headquarter.bytesPerLine() + 2 ] = 0xff;
                        ptr[ 4 * x + y * headquarter.bytesPerLine() + 3 ] = 0x00;
                    }
                    if( IsValid( karma, x, y ) )
                    {
                        ptr[ 4 * x + y * headquarter.bytesPerLine() ]     = 0x00;
                        ptr[ 4 * x + y * headquarter.bytesPerLine() + 1 ] = 0x00;
                        ptr[ 4 * x + y * headquarter.bytesPerLine() + 2 ] = 0x00;
                        ptr[ 4 * x + y * headquarter.bytesPerLine() + 3 ] = 0xff;
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
