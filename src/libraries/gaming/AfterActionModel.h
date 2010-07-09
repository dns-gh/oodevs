// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionModel_h_
#define __AfterActionModel_h_

#include "tools/Resolver.h"

namespace MsgsAarToClient
{
    class MsgAarInformation;
    class MsgPlotResult;
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
    void Update( const MsgsAarToClient::MsgAarInformation& message );
    void Update( const MsgsAarToClient::MsgPlotResult& message );
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
    void Load( const std::string& functions );
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
