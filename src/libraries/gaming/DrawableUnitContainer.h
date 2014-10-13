// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawableUnitContainer_h_
#define __DrawableUnitContainer_h_

namespace gui
{
    class Viewport_ABC;
    class GLView_ABC;
}

namespace kernel
{
    class Entity_ABC;
    class TacticalHierarchies;
}

// =============================================================================
/** @class  DrawableUnitContainer
    @brief  DrawableUnitContainer
*/
// Created: LDC 2013-04-14
// =============================================================================
class DrawableUnitContainer
{
public:
    //! @name Constructors/Destructor
    //@{
     DrawableUnitContainer();
    ~DrawableUnitContainer();
    //@}

    //! @name Extensions
    //@{
    void Draw( const kernel::Entity_ABC& entity, const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools, float factor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void InitializeSymbol( const kernel::TacticalHierarchies& hierarchies ) const;
    //@}

private:
    //! @name Member data
    //@{
    mutable std::string symbol_; // $$$$ RC LDC: code duplication with preparation
    mutable std::string staticSymbol_;
    mutable std::string moveSymbol_;
    mutable std::string level_;
    //@}
};

#endif // __DrawableUnitContainer_h_