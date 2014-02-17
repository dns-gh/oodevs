// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Line_h_
#define __Line_h_

#include "Node.h"

namespace svg
{

// =============================================================================
/** @class  Line
    @brief  Line
*/
// Created: AGE 2006-08-16
// =============================================================================
class Line : public Node
{

public:
    //! @name Constructors/Destructor
    //@{
             Line( xml::xistream& input, References_ABC& references, const PropertyFactory& factory );
    virtual ~Line();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Line& operator=( const Line& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    geometry::Point from_;
    geometry::Point to_;
    //@}
};

}

#endif // __Line_h_
