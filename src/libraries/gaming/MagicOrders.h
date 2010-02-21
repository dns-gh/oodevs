// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrders_h_
#define __MagicOrders_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace MsgsSimToClient
{
	class MsgUnitAttributes;
}

namespace kernel
{
    class Agent_ABC;
}

// =============================================================================
/** @class  MagicOrders
    @brief  MagicOrders. Pas terrible
*/
// Created: AGE 2006-04-28
// =============================================================================
class MagicOrders : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MagicOrders( const kernel::Agent_ABC& agent );
    virtual ~MagicOrders();
    //@}

    //! @name Operations
    //@{
    bool CanRetrieveTransporters() const;
    bool CanMagicMove() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MagicOrders( const MagicOrders& );            //!< Copy constructor
    MagicOrders& operator=( const MagicOrders& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Agent_ABC& agent_;
    bool transportersReady_;
    //@}
};

#endif // __MagicOrders_h_
