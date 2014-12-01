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
#include <tools/Resolver.h>

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
    @brief  EntitySymbols
*/
// Created: SBO 2007-02-21
// =============================================================================
class EntitySymbols : private boost::noncopyable
{
public:
    //! @name Type
    //@{
    enum ColorMode
    {
        eColorBase,
        eColorWithModifier,
        eColorSelected,
        eColorSuperiorSelected
    };
    //@}

    //! @name Constructors/Destructor
    //@{
             EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy );
    virtual ~EntitySymbols();
    //@}

    const QPixmap& GetSymbol( const kernel::Entity_ABC& entity, ColorMode colorMode = eColorBase ) const;
    const QPixmap& GetSymbol( const kernel::Entity_ABC& entity, const std::string& symbolName, const std::string& levelName,
        ColorMode colorMode = eColorBase ) const;

    void GenerateSymbols( const tools::Resolver< kernel::Team_ABC >& teamResolver ) const;
    //@}

private:
    //! @name Helper
    //@{
    void RecGenerateSymbols( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Member data
    //@{
    SymbolIcons& icons_;
    ColorStrategy_ABC& strategy_;
    //@}
};

}

#endif // __EntitySymbols_h_
