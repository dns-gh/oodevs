// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionModel_h_
#define __AfterActionModel_h_

#include "tools/Resolver.h"

namespace sword
{
    class AarInformation;
    class PlotResult;
}

namespace kernel
{
    class Controller;
}

namespace xml
{
    class xistream;
}

class AfterActionFunction;
class IndicatorRequest;
class IndicatorRequests;
class Publisher_ABC;

// =============================================================================
/** @class  AfterActionModel
    @brief  AfterActionModel
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionModel : public tools::Resolver< AfterActionFunction, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~AfterActionModel();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::AarInformation& message );
    void Update( const sword::PlotResult& message );
    void Purge();
    IndicatorRequest& CreateRequest( const AfterActionFunction& function );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionModel( const AfterActionModel& );            //!< Copy constructor
    AfterActionModel& operator=( const AfterActionModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Load( const tools::Path& functions );
    void ReadFunction( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::auto_ptr< IndicatorRequests > requests_;
    //@}
};

#endif // __AfterActionModel_h_
