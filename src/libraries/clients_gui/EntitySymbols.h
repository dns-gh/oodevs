// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySymbols_h_
#define __EntitySymbols_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Knowledge_ABC;
    class Team_ABC;
}

namespace gui
{
    class SymbolIcons;
    class ColorStrategy_ABC;

// =============================================================================
/** @class  EntitySymbols
    @brief  Entity symbols
*/
// Created: SBO 2007-02-21
// =============================================================================
class EntitySymbols : private boost::noncopyable
{
public:
    enum ColorMode
    {
        eColorBase,
        eColorWithModifier,
        eColorSelected,
        eColorSuperiorSelected
    };

public:
             EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy );
    virtual ~EntitySymbols();

    const QPixmap& GetSymbol( const kernel::Entity_ABC& entity, const QSize& size = QSize( 64, 64 ), ColorMode colorMode = eColorBase ) const;
    const QPixmap& GetSymbol( const kernel::Entity_ABC& entity, const std::string& symbolName, const std::string& levelName,
        const QSize& size = QSize( 64, 64 ), ColorMode colorMode = eColorBase ) const;

private:
    SymbolIcons& icons_;
    ColorStrategy_ABC& strategy_;
};

}

#endif // __EntitySymbols_h_
