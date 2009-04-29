// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Variable_h_
#define __Variable_h_

#include "Constant.h"

namespace xml
{
    class xistream;
}

namespace indicators
{
    class ElementType;

// =============================================================================
/** @class  Variable
    @brief  Variable
*/
// Created: SBO 2009-03-17
// =============================================================================
class Variable : public Constant< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             Variable( const std::string& name, const std::string& type, const std::string& value );
    explicit Variable( xml::xistream& xis );
    virtual ~Variable();
    //@}

    //! @name Operations
    //@{
    virtual Element_ABC& Clone() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Variable( const Variable& );            //!< Copy constructor
    Variable& operator=( const Variable& ); //!< Assignment operator
    //@}
};

}

#endif // __Variable_h_
