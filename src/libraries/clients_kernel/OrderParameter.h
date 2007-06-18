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
             OrderParameter( const QString& name, const QString& type, bool optional, bool context = false );
    virtual ~OrderParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetType() const;
    bool IsOptional() const;
    bool IsContext() const;
    const OrderParameterValue& GetValue( unsigned int id ) const;
    virtual void Accept( OrderParameterValueVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadValue( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, OrderParameterValue > T_OrderParameterValues;
    typedef T_OrderParameterValues::const_iterator      CIT_OrderParameterValues;
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    QString type_;
    bool optional_;
    bool context_;
    T_OrderParameterValues values_;
    //@}
};

}

#endif // __OrderParameter_h_
