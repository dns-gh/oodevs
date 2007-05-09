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

namespace dispatcher
{
    class Config;
    class Model;
    class ClientsNetworker;
    class SimulationDispatcher;
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
    explicit Replayer( const Config& config );
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
    Model*                pModel_;
    ClientsNetworker*     pClientsNetworker_;
    SimulationDispatcher* simulation_;
    LoaderFacade*         loader_;

    ProfileManager*       profiles_;
    //@}
};

}

#endif // __Replayer_h_
