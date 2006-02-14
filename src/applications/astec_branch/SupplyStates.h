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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class Controller;

// =============================================================================
/** @class  SupplyStates
    @brief  SupplyStates
*/
// Created: AGE 2006-02-14
// =============================================================================
class SupplyStates : public Extension_ABC
                   , public Updatable_ABC< ASN1T_MsgLogRavitaillementEtat >
                   , public Updatable_ABC< ASN1T_MsgLogRavitaillementQuotas >
{
    // $$$$ AGE 2006-02-14: entierement optionnel
public:
    //! @name Constructors/Destructor
    //@{
             SupplyStates( Controller& controller );
    virtual ~SupplyStates();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SupplyStates( const SupplyStates& );            //!< Copy constructor
    SupplyStates& operator=( const SupplyStates& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void Update( const ASN1T_MsgLogRavitaillementEtat& message );
    virtual void Update( const ASN1T_MsgLogRavitaillementQuotas& message );
    //@}

    //! @name Types
    //@{
    // $$$$ AGE 2006-02-14: resolve
    typedef std::map< unsigned long, unsigned >            T_ResourceQty_Map;
    typedef std::vector< std::pair< unsigned, unsigned > > T_LogisticAvailabilities;
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;

    bool                     bChainEnabled_;
    T_LogisticAvailabilities dispoTransporters_;
    T_LogisticAvailabilities dispoCommanders_;  
    T_ResourceQty_Map        stocks_;
    T_LogisticAvailabilities quotas_;
    //@}
};

#endif // __SupplyStates_h_
