// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderParameter_h_
#define __OrderParameter_h_

#include "Resolver.h"

namespace xml
{
    class xistream;
}

namespace kernel
{

// =============================================================================
/** @class  OrderParameter
    @brief  OrderParameter
*/
// Created: SBO 2007-05-31
// =============================================================================
class OrderParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderParameter( xml::xistream& xis );
    virtual ~OrderParameter();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    unsigned int GetTypeId() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderParameter( const OrderParameter& );            //!< Copy constructor
    OrderParameter& operator=( const OrderParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetTypeId();
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string type_;
    unsigned int typeId_;
    bool optional_;
    //@}
};

}

#endif // __OrderParameter_h_
