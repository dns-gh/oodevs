// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __OrderParameter_h_
#define __OrderParameter_h_

#include "OrderParameterValue.h"

namespace xml
{
    class xistream;
}

namespace kernel
{

class OrderParameterValueVisitor_ABC
{
public:
             OrderParameterValueVisitor_ABC() {}
    virtual ~OrderParameterValueVisitor_ABC() {}

    virtual void Visit( const OrderParameterValue& value ) = 0;
};

class ChoicesVisitor_ABC
{
public:
             ChoicesVisitor_ABC() {}
    virtual ~ChoicesVisitor_ABC() {}

    virtual void Visit( const std::string& type ) = 0;
};

// =============================================================================
/** @class  OrderParameter
    @brief  OrderParameter
*/
// Created: SBO 2007-04-23
// =============================================================================
class OrderParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderParameter( xml::xistream& xis );
             OrderParameter( const std::string& name, const std::string& type, bool optional );
    virtual ~OrderParameter();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    std::string GetType() const;
    bool IsOptional() const;
    bool IsList() const;
    const OrderParameterValue& GetValue( unsigned int id ) const;
    void AddValue( int id, const std::string& name );
    virtual void Accept( OrderParameterValueVisitor_ABC& visitor ) const;
    virtual void Accept( ChoicesVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadValue( xml::xistream& xis );
    void ReadChoice( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, OrderParameterValue > T_OrderParameterValues;
    typedef T_OrderParameterValues::const_iterator      CIT_OrderParameterValues;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string type_;
    bool optional_;
    unsigned int minOccurs_;
    unsigned int maxOccurs_;
    T_OrderParameterValues values_;
    std::vector< std::string > choices_;
    //@}
};

}

#endif // __OrderParameter_h_
