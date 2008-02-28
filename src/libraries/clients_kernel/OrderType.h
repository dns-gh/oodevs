// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderType_h_
#define __OrderType_h_

#include "Resolver.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class OrderParameter;

// =============================================================================
/** @class  OrderType
    @brief  OrderType
*/
// Created: SBO 2007-04-23
// =============================================================================
class OrderType : public Resolver< OrderParameter >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderType( xml::xistream& xis );
    virtual ~OrderType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderType( const OrderType& );            //!< Copy constructor
    OrderType& operator=( const OrderType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    //@}
};

}

#endif // __OrderType_h_
