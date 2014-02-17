// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DashArrayFactory_h_
#define __DashArrayFactory_h_

#include "PropertyFactory_ABC.h"
#include "DashArray.h"

namespace svg
{

// =============================================================================
/** @class  DashArrayFactory
    @brief  DashArrayFactory
*/
// Created: AGE 2006-09-06
// =============================================================================
class DashArrayFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DashArrayFactory();
    virtual ~DashArrayFactory();
    //@}

    //! @name Operations
    //@{
    virtual DashArray* CreateProperty( const std::string& content ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DashArrayFactory( const DashArrayFactory& );            //!< Copy constructor
    DashArrayFactory& operator=( const DashArrayFactory& ); //!< Assignement operator
    //@}
};

}

#endif // __DashArrayFactory_h_
