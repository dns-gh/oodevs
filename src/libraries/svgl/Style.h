// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Style_h_
#define __Style_h_

#include "PropertyGroup.h"
#include "Style_ABC.h"

namespace svg
{
    class PropertyFactory;

// =============================================================================
/** @class  Style
    @brief  Style
*/
// Created: AGE 2007-10-30
// =============================================================================
class Style : public Style_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Style( const std::string& css, const PropertyFactory& factory );
    virtual ~Style();
    //@}

    //! @name Operations
    //@{
    virtual void Push( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void Pop ( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Style( const Style& );            //!< Copy constructor
    Style& operator=( const Style& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    PropertyGroup properties_;
    //@}
};

}

#endif // __Style_h_
