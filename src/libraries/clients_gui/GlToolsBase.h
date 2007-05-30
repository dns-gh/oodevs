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

#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace gui
{
    class GLSymbols;

// =============================================================================
/** @class  GlToolsBase
    @brief  GlToolsBase
*/
// Created: AGE 2006-04-07
// =============================================================================
class GlToolsBase : public kernel::GlTools_ABC
                  , public kernel::Observer_ABC
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
    virtual bool Select( bool ) const;
    virtual bool ShouldDisplay( const std::string& name ) const;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const;
    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 );
    virtual std::auto_ptr< kernel::GlTooltip_ABC > CreateTooltip() const;

    GlToolsBase& Base() const;
    void BindIcon( const char** xpm );
    geometry::Point2f IconLocation( const char** xpm );
    void PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void SetIconLocation( const char** xpm, const geometry::Point2f& position );
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

    typedef std::map< std::string, kernel::TristateOption >  T_Options;
    typedef T_Options::iterator                             IT_Options;
    typedef T_Options::const_iterator                      CIT_Options;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    mutable bool selected_;

    T_Icons         icons_;
    GLSymbols*      symbols_;

    unsigned billboard_;

    mutable T_Options options_;
    //@}
};

}

#endif // __GlToolsBase_h_
