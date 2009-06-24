// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
    std::string GetDoctrineInformation() const;
    std::string GetUsageInformation() const;
    void AddParameter( const OrderParameter& parameter );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderType( const OrderType& );            //!< Copy constructor
    OrderType& operator=( const OrderType& ); //!< Assignment operator
    //@}

protected:
    //! @name Helpers
    //@{
    void ReadDescriptions( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    std::string doctrine_;
    std::string usage_;
    //@}
};

}

#endif // __OrderType_h_
