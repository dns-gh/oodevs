// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderContext_h_
#define __OrderContext_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    class OrderType;
    class OrderParameter;

// =============================================================================
/** @class  OrderContext
    @brief  OrderContext
*/
// Created: SBO 2008-03-05
// =============================================================================
class OrderContext
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OrderContext( xml::xistream& xis );
    virtual ~OrderContext();
    //@}

    //! @name Operations
    //@{
    void AddParameters( OrderType& type ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderContext( const OrderContext& );            //!< Copy constructor
    OrderContext& operator=( const OrderContext& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const OrderParameter* > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    T_Parameters parameters_;
    //@}
};

}

#endif // __OrderContext_h_
