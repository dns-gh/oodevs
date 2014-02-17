// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TextAnchorFactory_h_
#define __TextAnchorFactory_h_

#include "PropertyFactory_ABC.h"

namespace svg
{

// =============================================================================
/** @class  TextAnchorFactory
    @brief  TextAnchorFactory
*/
// Created: AGE 2006-10-20
// =============================================================================
class TextAnchorFactory : public PropertyFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TextAnchorFactory();
    virtual ~TextAnchorFactory();
    //@}

    //! @name Operations
    //@{
    virtual Property_ABC* CreateProperty( const std::string& content ) const;
    //@}
};

}

#endif // __TextAnchorFactory_h_
