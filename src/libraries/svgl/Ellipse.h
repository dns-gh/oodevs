// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Ellipse_h_
#define __Ellipse_h_

#include "Node.h"

namespace svg
{

// =============================================================================
/** @class  Ellipse
    @brief  Ellipse
*/
// Created: AGE 2006-08-15
// =============================================================================
class Ellipse : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Ellipse( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
             Ellipse( xml::xistream& input, References_ABC& references, const PropertyFactory& factory, const geometry::Point& center, float radius );
    virtual ~Ellipse();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Ellipse( const Ellipse& );            //!< Copy constructor
    Ellipse& operator=( const Ellipse& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void DrawInternal( int type, unsigned segments, float increment ) const;
    unsigned GetSegmentNumber( RenderingContext_ABC& context ) const;
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point center_;
    float rx_, ry_;
    float length_;
    //@}
};

}

#endif // __Ellipse_h_
