// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FontSizeFactory_h_
#define __FontSizeFactory_h_

#include "PropertyFactory_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FontSizeFactory
    @brief  FontSizeFactory
*/
// Created: AGE 2006-10-20
// =============================================================================
class FontSizeFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FontSizeFactory();
    virtual ~FontSizeFactory();
    //@}

    //! @name Operations
    //@{
    virtual Property_ABC* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FontSizeFactory( const FontSizeFactory& );            //!< Copy constructor
    FontSizeFactory& operator=( const FontSizeFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __FontSizeFactory_h_
