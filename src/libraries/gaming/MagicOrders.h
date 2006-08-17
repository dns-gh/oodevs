// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrders_h_
#define __MagicOrders_h_

#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Updatable_ABC.h"
#include "ASN_Types.h"

class Agent_ABC;

// =============================================================================
/** @class  MagicOrders
    @brief  MagicOrders
*/
// Created: AGE 2006-04-28
// =============================================================================
class MagicOrders : public Extension_ABC
                  , public Updatable_ABC< ASN1T_MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MagicOrders( const Agent_ABC& agent );
    virtual ~MagicOrders();
    //@}

    //! @name Operations
    //@{
    bool CanSurrender() const;
    bool CanMagicMove() const;
    bool CanRetrieveTransporters() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MagicOrders( const MagicOrders& );            //!< Copy constructor
    MagicOrders& operator=( const MagicOrders& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const Agent_ABC& agent_;
    bool transportersReady_;
    //@}
};

#endif // __MagicOrders_h_
