// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderParameterValue_h_
#define __OrderParameterValue_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    
// =============================================================================
/** @class  OrderParameterValue
    @brief  OrderParameterValue
*/
// Created: SBO 2007-04-23
// =============================================================================
class OrderParameterValue
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderParameterValue( xml::xistream& xis );
    virtual ~OrderParameterValue();
    //@}

    //! @name Operations
    //@{
    int GetId() const;
    QString GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderParameterValue( const OrderParameterValue& );            //!< Copy constructor
    OrderParameterValue& operator=( const OrderParameterValue& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    int id_;
    QString name_;
    //@}
};

}

#endif // __OrderParameterValue_h_
