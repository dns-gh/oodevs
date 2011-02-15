// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StockAttribute_ABC_h_
#define __StockAttribute_ABC_h_

#include "ObjectExtensions.h"
#include "OptionalValue.h"

namespace kernel
{
    class DotationType;

// =============================================================================
/** @class  Stock_ABC
    @brief  Gestion des stocks des objects
*/
// Created: BCI 2011-02-04
// =============================================================================
class StockAttribute_ABC : public ObjectExtension
{
public:
    //! @name Types
    //@{
    struct StockDotation
    {
        StockDotation()
            : maxStock_( 0  )
            , stock_( 0 )
        {}

        kernel::OptionalValue< unsigned int > stock_;
        kernel::OptionalValue< unsigned int > maxStock_;
    };
    typedef std::map< const kernel::DotationType*, StockDotation > StockDotations;
    //@}

    //! @name Constructors/Destructor
    //@{
             StockAttribute_ABC() {}
    virtual ~StockAttribute_ABC() {}
    //@}

    //! @name Operations
    //@{
    void SetDotation( const kernel::DotationType& dotationType, kernel::OptionalValue< unsigned int > stock, kernel::OptionalValue< unsigned int > maxStock );
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}
protected:
    //! @name Member data
    //@{
    StockDotations stockDotations_;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StockAttribute_ABC( const StockAttribute_ABC& );            //!< Copy constructor
    StockAttribute_ABC& operator=( const StockAttribute_ABC& ); //!< Assignment operator
    //@}

};

}

#endif // __StockAttribute_ABC_h_
