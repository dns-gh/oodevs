// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Explosions_h_
#define __Explosions_h_

#include "network/Simulation_Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

#include <deque>

namespace kernel
{
    class Controller;
}

class FireResultFactory;
class AgentFireResult;
class PopulationFireResult;

// =============================================================================
/** @class  Explosions
    @brief  Explosions
    // $$$$ AGE 2006-03-10: Aussi une extension d'agent. Renommer !
*/
// Created: AGE 2006-03-10
// =============================================================================
class Explosions : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< ASN1T_MsgExplosion >
                 , public kernel::Updatable_ABC< ASN1T_MsgStopPionFire >
                 , public kernel::Updatable_ABC< ASN1T_MsgStopPopulationFire >
{

public:
    //! @name Constructors/Destructor
    //@{
             Explosions( kernel::Controller& controller, FireResultFactory& factory );
    virtual ~Explosions();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Explosions( const Explosions& );            //!< Copy constructor
    Explosions& operator=( const Explosions& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::deque< AgentFireResult* >  T_AgentFires;
    typedef T_AgentFires::const_iterator  CIT_AgentFires;

    typedef std::deque< PopulationFireResult* >  T_PopulationFires;
    typedef T_PopulationFires::const_iterator  CIT_PopulationFires;
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void UpdateData( const T& message );
    virtual void DoUpdate( const ASN1T_MsgExplosion& message );
    virtual void DoUpdate( const ASN1T_MsgStopPionFire& message );
    virtual void DoUpdate( const ASN1T_MsgStopPopulationFire& message );

    void Update( const ASN1T_FireDamagesPion& message );
    void Update( const ASN1T_FireDamagesPopulation& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    FireResultFactory& factory_;

    T_AgentFires       agentExplosions_;
    T_PopulationFires  populationExplosions_;
    //@}
};

#endif // __Explosions_h_
