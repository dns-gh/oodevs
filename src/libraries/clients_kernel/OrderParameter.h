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
    class OrderParameterValue;

// =============================================================================
/** @class  OrderParameter
    @brief  OrderParameter
*/
// Created: SBO 2007-04-23
// =============================================================================
class OrderParameter : public Resolver< OrderParameterValue >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderParameter( xml::xistream& xis );
             OrderParameter( const QString& name, const QString& type, bool optional );
    virtual ~OrderParameter();
    //@}

    //! @name Operations
    //@{
    QString GetName() const;
    QString GetType() const;
    bool IsOptional() const;
    bool IsContext() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderParameter( const OrderParameter& );            //!< Copy constructor
    OrderParameter& operator=( const OrderParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadValue( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    QString type_;
    bool optional_;
    bool context_;
    //@}
};

}

#endif // __OrderParameter_h_
