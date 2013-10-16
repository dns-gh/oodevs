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

#include "clients_kernel/StockAttribute_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Controller;
    class Automat_ABC;
    class Displayer_ABC;
    class DotationType;
}

namespace sword
{
    class ObjectKnowledgeUpdate;
    class ObjectUpdate;
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

private:
    //! @name Copy/Assignment
    //@{
    StockAttribute( const StockAttribute& );            //!< Copy constructor
    StockAttribute& operator=( const StockAttribute& ); //!< Assignment operator
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
    //@}
};

#endif // __StockAttribute_h_
