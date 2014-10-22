// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlToolsBase_h_
#define __GlToolsBase_h_

#include "GLView_ABC.h"

namespace tools
{
    class ExerciseConfig;
}

namespace gui
{
    class DrawingTypes;
    class GLSymbols;
    class SvglRenderer;
    class SvglProxy;
    class TacticalGraphics;

// =============================================================================
/** @class  GlToolsBase
    @brief  GlToolsBase
*/
// Created: AGE 2006-04-07
// =============================================================================
class GlToolsBase : public gui::GLView_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GlToolsBase( GLView_ABC& parent, const DrawingTypes& drawingTypes );
    virtual ~GlToolsBase();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const tools::ExerciseConfig& config );

    virtual float GetCurrentAlpha() const;
    virtual void SetCurrentColor( float r, float g, float b, float a = 1 );
    virtual std::unique_ptr< GlTooltip_ABC > CreateTooltip() const;

    GlToolsBase& Base() const;
    void BindIcon( const char** xpm );
    static std::string DefaultStyle();
    void PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480, bool checkAlpha = true );
    void DrawSvg  ( const std::string& filename, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport,
                               bool overlined, float pointSize, float zoom );
    void DrawBillboardRect();
    //@}

    //! @name Options
    //@{
    virtual GLOptions& GetOptions();
    virtual const GLOptions& GetOptions() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const char**, unsigned int > T_Icons;
    //@}

private:
    //! @name Member data
    //@{
    gui::GLView_ABC& parent_;
    T_Icons icons_;
    std::unique_ptr< SvglRenderer > renderer_;
    std::unique_ptr< GLSymbols > symbols_;
    std::unique_ptr< SvglProxy > svgl_;
    std::unique_ptr< TacticalGraphics > graphics_;
    unsigned int billboard_;
    float alpha_;
    //@}
};

}

#endif // __GlToolsBase_h_
