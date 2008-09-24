// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OrderProcessor_h_
#define __OrderProcessor_h_

#include "ResponseHandler_ABC.h"

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Model;
    class SimulationPublisher_ABC;
}

namespace plugins
{
namespace bml
{

// =============================================================================
/** @class  OrderProcessor
    @brief  OrderProcessor
*/
// Created: SBO 2008-05-22
// =============================================================================
class OrderProcessor : public ResponseHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OrderProcessor( const dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~OrderProcessor();
    //@}

    //! @name Operations
    //@{
    virtual void Handle( const std::string& response );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OrderProcessor( const OrderProcessor& );            //!< Copy constructor
    OrderProcessor& operator=( const OrderProcessor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadOrder( xml::xistream& xis );
    void ReadTask( xml::xistream& xis );
    void ReadGroundTask( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Model& model_;
    dispatcher::SimulationPublisher_ABC& publisher_;
    //@}
};

}
}

#endif // __OrderProcessor_h_
