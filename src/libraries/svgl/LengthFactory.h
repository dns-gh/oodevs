// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LengthFactory_h_
#define __LengthFactory_h_

#include "PropertyFactory_ABC.h"
#include "Length.h"

namespace svg
{

// =============================================================================
/** @class  LengthFactory
    @brief  LengthFactory
*/
// Created: AGE 2006-09-06
// =============================================================================
class LengthFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LengthFactory();
    virtual ~LengthFactory();
    //@}

    //! @name Operations
    //@{
    virtual Length* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LengthFactory( const LengthFactory& );            //!< Copy constructor
    LengthFactory& operator=( const LengthFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __LengthFactory_h_
