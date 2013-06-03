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

#include "GlTools_ABC.h"

#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace tools
{
    class ExerciseConfig;
}

namespace gui
{
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
class GlToolsBase : public gui::GlTools_ABC
                  , public tools::Observer_ABC
                  , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GlToolsBase( kernel::Controllers& controllers );
    virtual ~GlToolsBase();
    //@}

    //! @name Operations
    //@{
    virtual void Load( const tools::ExerciseConfig& config );
    virtual boost::tuple< bool, bool, bool > UnSelect() const;
    virtual void Select( bool, bool, bool ) const;

    virtual bool ShouldDisplay( const std::string& name = std::string() ) const;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const;
    virtual bool ShouldDisplay( const std::string& name, bool b1, bool b2, bool b3 ) const;
    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 );
    virtual std::auto_ptr< GlTooltip_ABC > CreateTooltip() const;

    GlToolsBase& Base() const;
    void BindIcon( const char** xpm );
    static std::string DefaultStyle();
    void PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void DrawSvg  ( const std::string& filename, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, bool overlined, float zoom );
    void DrawBillboardRect();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const char**, unsigned int > T_Icons;
    typedef T_Icons::const_iterator              CIT_Icons;

    typedef std::map< std::string, kernel::FourStateOption > T_Options;
    typedef T_Options::iterator                             IT_Options;
    typedef T_Options::const_iterator                      CIT_Options;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    mutable bool selected_;
    mutable bool superiorSelected_;
    mutable bool controlled_;
    T_Icons icons_;
    std::auto_ptr< SvglRenderer > renderer_;
    std::auto_ptr< GLSymbols > symbols_;
    std::auto_ptr< SvglProxy > svgl_;
    std::auto_ptr< TacticalGraphics > graphics_;
    unsigned int billboard_;
    mutable T_Options options_;
    //@}
};

}

#endif // __GlToolsBase_h_
