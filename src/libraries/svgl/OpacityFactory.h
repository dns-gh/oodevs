// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OpacityFactory_h_
#define __OpacityFactory_h_

#include "PropertyFactory_ABC.h"
#include "Opacity.h"

namespace svg
{

// =============================================================================
/** @class  OpacityFactory
    @brief  OpacityFactory
*/
// Created: AGE 2006-08-23
// =============================================================================
class OpacityFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OpacityFactory();
    virtual ~OpacityFactory();
    //@}

    //! @name Operations
    //@{
    virtual Opacity* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    OpacityFactory( const OpacityFactory& );            //!< Copy constructor
    OpacityFactory& operator=( const OpacityFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __OpacityFactory_h_
