// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyStates_h_
#define __SupplyStates_h_

#include "ASN_Types.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "Availability.h"
#include "Dotation.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
    class EquipmentType;
    class DotationType;
    class Displayer_ABC;
    class DataDictionary;
}

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyStates : public kernel::Extension_ABC
                   , public kernel::Updatable_ABC< ASN1T_MsgLogRavitaillementEtat >
                   , public kernel::Updatable_ABC< ASN1T_MsgLogRavitaillementQuotas >
                   , public kernel::Resolver< Dotation >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::EquipmentType >& resolver, const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver, kernel::DataDictionary& dico );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DoUpdate( const ASN1T_MsgLogRavitaillementEtat& message );
    virtual void DoUpdate( const ASN1T_MsgLogRavitaillementQuotas& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SupplyStates( const SupplyStates& );            //!< Copy constructor
    SupplyStates& operator=( const SupplyStates& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< Availability >         T_Availabilities;
    typedef std::vector< Dotation >             T_Dotations;
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::EquipmentType >& resolver_;
    const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver_;

    bool             bChainEnabled_;
    T_Availabilities dispoTransporters_;
    T_Dotations      quotas_;
    //@}
};

#endif // __SupplyStates_h_
