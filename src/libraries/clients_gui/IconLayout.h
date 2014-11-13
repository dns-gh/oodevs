// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IconLayout_h_
#define __IconLayout_h_

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  IconLayout
    @brief  IconLayout
*/
// Created: SBO 2006-08-18
// =============================================================================
class IconLayout : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             IconLayout();
    virtual ~IconLayout();
    //@}

    //! @name Operations
    //@{
    void AddIcon( const char** xpm, int x, int y );
    const geometry::Point2f& IconLocation( const char** xpm ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::map< const char**, geometry::Point2f > iconLocations_;
    //@}
};

} //! namespace gui

#endif // __IconLayout_h_
