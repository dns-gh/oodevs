// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TooltipsLayer_h_
#define __TooltipsLayer_h_

#include "TooltipsLayer_ABC.h"

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{

// =============================================================================
/** @class  TooltipsLayer
    @brief  TooltipsLayer
*/
// Created: SBO 2008-04-11
// =============================================================================
class TooltipsLayer : public TooltipsLayer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TooltipsLayer( kernel::GlTools_ABC& tools );
    virtual ~TooltipsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( kernel::Viewport_ABC& viewport );
    virtual void Draw( const geometry::Point2f& position, const QImage& image );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TooltipsLayer( const TooltipsLayer& );            //!< Copy constructor
    TooltipsLayer& operator=( const TooltipsLayer& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::pair< geometry::Point2f, QImage > > T_Tooltips;
    typedef T_Tooltips::const_iterator                          CIT_Tooltips;
    //@}

private:
    //! @name Member data
    //@{
    kernel::GlTools_ABC& tools_;
    T_Tooltips tooltips_;
    //@}
};

}

#endif // __TooltipsLayer_h_
