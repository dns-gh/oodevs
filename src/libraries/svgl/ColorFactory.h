// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ColorFactory_h_
#define __ColorFactory_h_

#include "PropertyFactory_ABC.h"
#include "Paint_ABC.h"

namespace svg
{

// =============================================================================
/** @class  ColorFactory
    @brief  ColorFactory
*/
// Created: AGE 2006-08-23
// =============================================================================
class ColorFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ColorFactory();
    virtual ~ColorFactory();
    //@}

    //! @name Operations
    //@{
    virtual Paint_ABC* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ColorFactory( const ColorFactory& );            //!< Copy constructor
    ColorFactory& operator=( const ColorFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __ColorFactory_h_
