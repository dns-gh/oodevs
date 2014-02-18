// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FontWeightFactory_h_
#define __FontWeightFactory_h_

#include "PropertyFactory_ABC.h"

namespace svg
{

// =============================================================================
/** @class  FontWeightFactory
    @brief  FontWeightFactory
*/
// Created: AGE 2006-10-20
// =============================================================================
class FontWeightFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FontWeightFactory();
    virtual ~FontWeightFactory();
    //@}

    //! @name Operations
    //@{
    virtual Property_ABC* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FontWeightFactory( const FontWeightFactory& );            //!< Copy constructor
    FontWeightFactory& operator=( const FontWeightFactory& ); //!< Assignement operator
    //@}
};
}

#endif // __FontWeightFactory_h_
