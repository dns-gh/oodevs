// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __GlToolsBase_h_
#define __GlToolsBase_h_

#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/OptionsObserver_ABC.h"

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
class GlToolsBase : public kernel::GlTools_ABC
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
    virtual std::pair< bool, bool > UnSelect() const;
    virtual void Select( bool, bool ) const;

    virtual bool ShouldDisplay( const std::string& name = std::string() ) const;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const;
    virtual bool ShouldDisplay( const std::string& name, bool b1, bool b2 ) const;
    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 );
    virtual std::auto_ptr< kernel::GlTooltip_ABC > CreateTooltip() const;

    GlToolsBase& Base() const;
    void BindIcon( const char** xpm );
    static std::string DefaultStyle();
    void PrintApp6( const std::string& symbol, const std::string& style, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void DrawSvg  ( const std::string& filename, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, const geometry::Rectangle2f& viewport, bool overlined );
    void DrawBillboardRect();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GlToolsBase( const GlToolsBase& );            //!< Copy constructor
    GlToolsBase& operator=( const GlToolsBase& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const char**, unsigned >            T_Icons;
    typedef T_Icons::const_iterator                     CIT_Icons;

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

    T_Icons         icons_;
    std::auto_ptr< SvglRenderer > renderer_;
    std::auto_ptr< GLSymbols >    symbols_;
    std::auto_ptr< SvglProxy >    svgl_;
    std::auto_ptr< TacticalGraphics > graphics_;

    unsigned billboard_;

    mutable T_Options options_;
    //@}
};

}

#endif // __GlToolsBase_h_
