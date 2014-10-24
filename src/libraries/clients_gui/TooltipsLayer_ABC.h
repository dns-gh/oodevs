// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TooltipsLayer_ABC_h_
#define __TooltipsLayer_ABC_h_

namespace gui
{
    class Tooltip;

// =============================================================================
/** @class  TooltipsLayer_ABC
    @brief  TooltipsLayer_ABC
*/
// Created: SBO 2008-04-11
// =============================================================================
class TooltipsLayer_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TooltipsLayer_ABC() {}
    virtual ~TooltipsLayer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& position, const Tooltip& tooltip ) = 0;
    //@}
};

}

#endif // __TooltipsLayer_ABC_h_
