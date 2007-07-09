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

#include "CompositeMessageHandler.h"
#include "boost/shared_ptr.hpp"

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
    explicit Dispatcher( const Config& config );
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
    const Config&                             config_;
    CompositeMessageHandler                   handler_;
    boost::shared_ptr< Model >                pModel_;
    boost::shared_ptr< ClientsNetworker >     pClientsNetworker_;
    boost::shared_ptr< SimulationNetworker >  pSimulationNetworker_;
    boost::shared_ptr< ProfileManager >       pProfileManager_;
};

}

#include "Dispatcher.inl"

#endif // __Dispatcher_h_
