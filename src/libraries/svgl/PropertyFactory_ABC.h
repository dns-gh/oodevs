// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PropertyFactory_ABC_h_
#define __PropertyFactory_ABC_h_

namespace svg
{
    class Property_ABC;

// =============================================================================
/** @class  PropertyFactory_ABC
    @brief  PropertyFactory_ABC
*/
// Created: AGE 2006-08-23
// =============================================================================
class PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PropertyFactory_ABC() {};
    virtual ~PropertyFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual Property_ABC* CreateProperty( const std::string& content ) const = 0;
    //@}
};

}

#endif // __PropertyFactory_ABC_h_
