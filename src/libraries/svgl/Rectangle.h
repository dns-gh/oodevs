// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Rectangle_h_
#define __Rectangle_h_

#include "Node.h"

namespace svg
{

// =============================================================================
/** @class  Rectangle
    @brief  Rectangle
*/
// Created: AGE 2006-08-15
// =============================================================================
class Rectangle : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Rectangle( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~Rectangle();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Rectangle( const Rectangle& );            //!< Copy constructor
    Rectangle& operator=( const Rectangle& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    float x1_, y1_;
    float x2_, y2_;
    //@}
};

}

#endif // __Rectangle_h_
