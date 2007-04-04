// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Quotas_h_
#define __Quotas_h_

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver_ABC.h"
#include "Dotation.h"

namespace kernel
{
    class Controller;
    class DotationType;
    class Displayer_ABC;
}


// =============================================================================
/** @class  Quotas
    @brief  Quotas
*/
// Created: AGE 2006-10-11
// =============================================================================
class Quotas  : public kernel::Extension_ABC
              , public kernel::Updatable_ABC< ASN1T_MsgLogRavitaillementQuotas >
{

public:
    //! @name Constructors/Destructor
    //@{
             Quotas(  kernel::Controller& controller, const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver );
    virtual ~Quotas();
    //@}

    //! @name Operations
    //@{
    bool IsRelevant() const;
    virtual void DoUpdate( const ASN1T_MsgLogRavitaillementQuotas& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Quotas( const Quotas& );            //!< Copy constructor
    Quotas& operator=( const Quotas& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Dotation >             T_Dotations;
    //@}    

public: // $$$$ AGE 2006-10-11: 
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver_; 
    T_Dotations      quotas_;
    //@}
};

#endif // __Quotas_h_
