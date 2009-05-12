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
    class DataTypeFactory;

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
             Variable( const std::string& name, boost::shared_ptr< DataType_ABC > type, const std::string& value );
             Variable( const Element_ABC& element, const DataTypeFactory& types );
             Variable( xml::xistream& xis, const DataTypeFactory& types );
    virtual ~Variable();
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
