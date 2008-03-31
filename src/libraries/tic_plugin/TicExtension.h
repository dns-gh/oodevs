// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TicExtension_h_
#define __TicExtension_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "game_asn/Simulation.h"
#pragma warning (disable : 4511 4512 4127 )
#include <boost/ptr_container/ptr_vector.hpp>

namespace dispatcher
{
    class Agent;
}

namespace tic
{
    class Platform;

// =============================================================================
/** @class  TicExtension
    @brief  TicExtension
*/
// Created: AGE 2008-03-31
// =============================================================================
class TicExtension : public kernel::Extension_ABC
                   , public kernel::Updatable_ABC< ASN1T_MsgUnitAttributes >
                   , public kernel::Updatable_ABC< ASN1T_MsgUnitEnvironmentType >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TicExtension( dispatcher::Agent& holder );
    virtual ~TicExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& updateMessage );
    virtual void DoUpdate( const ASN1T_MsgUnitEnvironmentType& updateMessage );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TicExtension( const TicExtension& );            //!< Copy constructor
    TicExtension& operator=( const TicExtension& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreatePlatforms();
    void UpdatePlatforms( const ASN1T_MsgUnitAttributes& updateMessage );
    void UpdatePlatforms( const ASN1T_EquipmentDotations& updateMessage );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent& holder_;
    boost::ptr_vector< Platform > platforms_;
    bool onRoad_;
    //@}
};

}

#endif // __TicExtension_h_
