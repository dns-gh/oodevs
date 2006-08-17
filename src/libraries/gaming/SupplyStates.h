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
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Updatable_ABC.h"
#include "Availability.h"
#include "Dotation.h"
#include "astec_kernel/Resolver.h"

class Controller;
class EquipmentType;
class DotationType;
class Displayer_ABC;
class DataDictionary;

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyStates : public Extension_ABC
                   , public Updatable_ABC< ASN1T_MsgLogRavitaillementEtat >
                   , public Updatable_ABC< ASN1T_MsgLogRavitaillementQuotas >
                   , public Resolver< Dotation >
{
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, const Resolver_ABC< DotationType >& dotationResolver, DataDictionary& dico );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
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
    Controller& controller_;
    const Resolver_ABC< EquipmentType >& resolver_;
    const Resolver_ABC< DotationType >& dotationResolver_;

    bool             bChainEnabled_;
    T_Availabilities dispoTransporters_;
    T_Dotations      quotas_;
    //@}
};

#endif // __SupplyStates_h_
