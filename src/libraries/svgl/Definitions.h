// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Definitions_h_
#define __Definitions_h_

#include "Container.h"

namespace svg 
{
    class Style;

// =============================================================================
/** @class  Definitions
    @brief  Definitions
*/
// Created: AGE 2006-08-10
// =============================================================================
class Definitions : public Container
{

public:
    //! @name Constructors/Destructor
    //@{
             Definitions( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory );
    virtual ~Definitions();
    //@}

private:
    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Definitions( const Definitions& );            //!< Copy constructor
    Definitions& operator=( const Definitions& ); //!< Assignement operator
    //@}
};

}

#endif // __Definitions_h_
