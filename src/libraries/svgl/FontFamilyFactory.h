// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FontFamilyFactory_h_
#define __FontFamilyFactory_h_

#include "PropertyFactory_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FontFamilyFactory
    @brief  FontFamilyFactory
*/
// Created: AGE 2006-10-20
// =============================================================================
class FontFamilyFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FontFamilyFactory();
    virtual ~FontFamilyFactory();
    //@}

    //! @name Operations
    //@{
    virtual Property_ABC* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FontFamilyFactory( const FontFamilyFactory& );            //!< Copy constructor
    FontFamilyFactory& operator=( const FontFamilyFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __FontFamilyFactory_h_
