// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Replayer_h_
#define __Replayer_h_

#include "CompositeMessageHandler.h"
#include <memory>
#include <boost/shared_ptr.hpp>

namespace dispatcher
{
    class Config;
    class Model;
    class ClientsNetworker;
    class SimulationDispatcher;
    class Loader;
    class LoaderFacade;
    class ProfileManager;

// =============================================================================
/** @class  Replayer
    @brief  Replayer
*/
// Created: AGE 2007-04-10
// =============================================================================
class Replayer
{

public:
    //! @name Constructors/Destructor
    //@{
             Replayer( const Config& config, const std::string& records );
    virtual ~Replayer();
    //@}

    //! @name Operations
    //@{
    void Update();
    //@}

    //! @name Accessors
    //@{
    Model&          GetModel   () const;
    ProfileManager& GetProfiles() const;
    LoaderFacade&   GetLoader  () const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Replayer( const Replayer& );            //!< Copy constructor
    Replayer& operator=( const Replayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    CompositeMessageHandler                   handler_;
    boost::shared_ptr< Model >                model_;
    std::auto_ptr< ClientsNetworker >         clientsNetworker_;
    boost::shared_ptr< SimulationDispatcher > simulation_;
    std::auto_ptr< Loader >                   loader_;
    std::auto_ptr< LoaderFacade >             facade_;
    std::auto_ptr< ProfileManager >           profiles_;
    //@}
};

}

#endif // __Replayer_h_
