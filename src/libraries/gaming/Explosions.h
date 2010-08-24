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

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace MsgsSimToClient
{
    class MsgExplosion;
    class MsgStopUnitFire;
    class MsgStopPopulationFire;
    class MsgUnitFireDamages;
    class MsgPopulationFireDamages;
}

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
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgExplosion >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgStopUnitFire >
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgStopPopulationFire >
{
public:
    //! @name Constructors/Destructor
    //@{
             Explosions( kernel::Controller& controller, FireResultFactory& factory );
    virtual ~Explosions();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Explosions( const Explosions& );            //!< Copy constructor
    Explosions& operator=( const Explosions& ); //!< Assignment operator
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
    virtual void DoUpdate( const MsgsSimToClient::MsgExplosion& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgStopUnitFire& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgStopPopulationFire& message );

    void Update( const MsgsSimToClient::MsgUnitFireDamages& message );
    void Update( const MsgsSimToClient::MsgPopulationFireDamages& message );
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
