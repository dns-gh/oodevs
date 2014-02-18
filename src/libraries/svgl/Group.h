// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Group_h_
#define __Group_h_

#include "Container.h"

namespace svg 
{
    class Style;

// =============================================================================
/** @class  Group
    @brief  Group
*/
// Created: AGE 2006-08-10
// =============================================================================
class Group : public Container
{

public:
    //! @name Constructors/Destructor
    //@{
             Group( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory );
    virtual ~Group();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Group( const Group& );            //!< Copy constructor
    Group& operator=( const Group& ); //!< Assignement operator
    //@}
};

}

#endif // __Group_h_
