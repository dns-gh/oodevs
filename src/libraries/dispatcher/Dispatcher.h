// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dispatcher_h_
#define __Dispatcher_h_

struct ASN1T_MsgsInClient;
struct ASN1T_MsgsInSim;

namespace DIN
{
    class DIN_Input;
}

namespace dispatcher
{
class Config;
class Model;
class SimulationNetworker;
class ClientsNetworker;
class ProfileManager;

// =============================================================================
/** @class  Dispatcher
    @brief  Dispatcher
*/
// Created: NLD 2006-09-19
// =============================================================================
class Dispatcher
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Dispatcher( Config& config );
    virtual ~Dispatcher();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    Model&               GetModel              () const;
    ClientsNetworker&    GetClientsNetworker   () const;
    SimulationNetworker& GetSimulationNetworker() const;
    ProfileManager&      GetProfileManager     () const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dispatcher( const Dispatcher& );            //!< Copy constructor
    Dispatcher& operator=( const Dispatcher& ); //!< Assignement operator
    //@}

private:
    Model*               pModel_;
    SimulationNetworker* pSimulationNetworker_;
    ClientsNetworker*    pClientsNetworker_;
    ProfileManager*      pProfileManager_;
};

}

#include "Dispatcher.inl"

#endif // __Dispatcher_h_
