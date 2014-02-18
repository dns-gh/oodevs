// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompiledEllipse_h_
#define __CompiledEllipse_h_

#include "Node.h"

namespace svg
{

// =============================================================================
/** @class  CompiledEllipse
    @brief  CompiledEllipse
*/
// Created: AGE 2006-09-08
// =============================================================================
class CompiledEllipse : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             CompiledEllipse( const Node& node, const geometry::Point& center, float rx, float ry, unsigned nSegments );
    virtual ~CompiledEllipse();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CompiledEllipse& operator=( const CompiledEllipse& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< geometry::Point > T_Points;
    //@}

    //! @name Helpers
    //@{
    void LockArray() const;
    //@}

private:
    //! @name Member data
    //@{
    T_Points points_;
    //@}
};

}

#endif // __CompiledEllipse_h_
