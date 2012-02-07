// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySymbols_h_
#define __EntitySymbols_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Intelligence_ABC;
    class Knowledge_ABC;
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
    //! @name Constructors/Destructor
    //@{
             EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy );
    virtual ~EntitySymbols();
    //@}

    //! @name Operations
    //@{
    const QPixmap& GetSymbol( const kernel::Entity_ABC& entity, const QSize& size = QSize( 32, 32 ) ) const;
    const QPixmap& GetSymbol( const kernel::Entity_ABC& entity, const std::string& symbolName, const std::string& levelName,
                              const QSize& size = QSize( 32, 32 ) ) const;
    QPixmap GetSymbol( const kernel::Entity_ABC& entity, const QPixmap& pixmap ) const;
    //@}

private:
    //! @name Helpers
    //@{
    bool IsCommandPost( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Statics
    //@{
    static const unsigned int headquarterSize_ = 8;
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
