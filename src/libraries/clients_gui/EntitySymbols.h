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
class EntitySymbols
{
public:
    //! @name Constructors/Destructor
    //@{
             EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy );
    virtual ~EntitySymbols();
    //@}

    //! @name Operations
    //@{
    virtual QPixmap GetSymbol( const kernel::Entity_ABC& entity, const QSize& size = QSize( 32, 32 ) ) const;
    virtual QPixmap GetSymbol( const kernel::Intelligence_ABC& entity, const QSize& size = QSize( 32, 32 ) ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntitySymbols( const EntitySymbols& );            //!< Copy constructor
    EntitySymbols& operator=( const EntitySymbols& ); //!< Assignment operator
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
