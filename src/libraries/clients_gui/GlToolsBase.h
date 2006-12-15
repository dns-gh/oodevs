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

namespace gui
{
    class GlFont;
    class GLSymbols;

// =============================================================================
/** @class  GlToolsBase
    @brief  GlToolsBase
*/
// Created: AGE 2006-04-07
// =============================================================================
class GlToolsBase : public kernel::GlTools_ABC
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

    GlToolsBase& Base() const;
    void BindIcon( const char** xpm );
    geometry::Point2f IconLocation( const char** xpm );
    void PrintApp6( const std::string& symbol, const geometry::Rectangle2f& viewport, unsigned vWidth = 640, unsigned vHeight = 480 );
    void SetIconLocation( const char** xpm, const geometry::Point2f& position );
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
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    mutable bool selected_;

    T_Icons         icons_;
    GLSymbols*      symbols_;
    //@}
};

}

#endif // __GlToolsBase_h_
