// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __gui_Tesselator_h_
#define __gui_Tesselator_h_

#include "clients_kernel/Types.h"
#include <boost/noncopyable.hpp>

class GLUtesselator;

namespace gui
{
// =============================================================================
/** @class  Tesselator
    @brief  Tesselator
*/
// Created: LGY 2014-10-10
// =============================================================================
class Tesselator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Tesselator();
    virtual ~Tesselator();
    //@}

    //! @name Operations
    //@{
    void Begin( unsigned int type );
    void Vertex( const geometry::Point2d& point );
    kernel::T_TessellatedPolygon Tessellate( const geometry::Polygon2f& polygon );
    //@}

private:
    //! @name Member data
    //@{
    GLUtesselator* pTesselatorBase_;
    kernel::T_TessellatedPolygon dataOut_;
    //@}
};

} //! namespace gui

#endif // __gui_Tesselator_h_
