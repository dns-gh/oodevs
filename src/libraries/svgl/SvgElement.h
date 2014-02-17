// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SvgElement_h_
#define __SvgElement_h_

#include "Container.h"

namespace svg 
{

// =============================================================================
/** @class  SvgElement
    @brief  SvgElement
*/
// Created: AGE 2006-08-10
// =============================================================================
class SvgElement : public Container
{

public:
    //! @name Constructors/Destructor
    //@{
             SvgElement( xml::xistream& input, const Factory_ABC& factory, References_ABC& references, const PropertyFactory& propertyFactory );
    virtual ~SvgElement();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SvgElement( const SvgElement& );            //!< Copy constructor
    SvgElement& operator=( const SvgElement& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __SvgElement_h_
