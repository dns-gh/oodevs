// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __StockAttribute_h_
#define __StockAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Displayer_ABC;
    class DotationType;
}

namespace sword
{
    class StockResource;
}

// =============================================================================
/** @class  StockAttribute
    @brief  StockAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class StockAttribute : public kernel::StockAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StockAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~StockAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StockAttribute( const StockAttribute& );            //!< Copy constructor
    StockAttribute& operator=( const StockAttribute& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef kernel::OptionalValue< unsigned int >   T_DotationState;
    typedef std::map< const kernel::DotationType*, std::pair< T_DotationState, T_DotationState > > T_StockDotation;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );

    template< typename T >
    void UpdateData( const T& message );
    void Update( const kernel::DotationType& type, const sword::StockResource& resource );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::DotationType >& resolver_;
    T_StockDotation stock_;
    //@}
};

#endif // __StockAttribute_h_
