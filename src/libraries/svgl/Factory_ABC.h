// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Factory_ABC_h_
#define __Factory_ABC_h_

namespace xml {
    class xistream;
}

namespace svg
{
    class Node_ABC;
    class References_ABC;

// =============================================================================
/** @class  Factory_ABC
    @brief  Factory_ABC
*/
// Created: AGE 2006-08-29
// =============================================================================
class Factory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Factory_ABC() {};
    virtual ~Factory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual Node_ABC* Create( const std::string& name, xml::xistream& input, svg::References_ABC& references ) const = 0;
    //@}
};

}

#endif // __Factory_ABC_h_
