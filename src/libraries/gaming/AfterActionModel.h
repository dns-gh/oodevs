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

#include "clients_kernel/Resolver.h"
#include "game_asn/asn.h"

namespace kernel
{
    class Controller;
}
namespace xml
{
    class xistream;
}

class AfterActionFactory;
class AfterActionFunction;
class AfterActionFunctions;
class AfterActionRequests;
class Publisher_ABC;

// =============================================================================
/** @class  AfterActionModel
    @brief  AfterActionModel
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionModel : public kernel::Resolver< AfterActionFactory, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionModel( kernel::Controller& controller, Publisher_ABC& publisher );
    virtual ~AfterActionModel();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgAarInformation& asnMsg );
    void Update( const ASN1T_MsgIndicatorResult& asnMsg );

    const AfterActionFunction* CreateNewFunction();
    void CreateRequest( const AfterActionFunction& function );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionModel( const AfterActionModel& );            //!< Copy constructor
    AfterActionModel& operator=( const AfterActionModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadFunction( const std::string& type, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    std::auto_ptr< AfterActionFunctions > functions_;
    std::auto_ptr< AfterActionRequests > requests_;
    //@}
};

#endif // __AfterActionModel_h_
