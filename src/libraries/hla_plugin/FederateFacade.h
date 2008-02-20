// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FederateFacade_h_
#define __FederateFacade_h_

class RtiAmbassador_ABC;
class Federate;
class TimeFactory_ABC;
class TimeIntervalFactory_ABC;
template< typename ObjectClass > class Class;

namespace dispatcher
{
    class Agent;
}

namespace hla
{
    class AgentExtension;

// =============================================================================
/** @class  FederateFacade
    @brief  FederateFacade
*/
// Created: SBO 2008-02-18
// =============================================================================
class FederateFacade
{

public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( const std::string& name, unsigned int timeStepDuration );
    virtual ~FederateFacade();
    //@}

    //! @name Operations
    //@{
    bool Join( const std::string& name );
    void Register( dispatcher::Agent& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FederateFacade( const FederateFacade& );            //!< Copy constructor
    FederateFacade& operator=( const FederateFacade& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeClasses();
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< TimeFactory_ABC >         timeFactory_;
    std::auto_ptr< TimeIntervalFactory_ABC > intervalFactory_;
    std::auto_ptr< RtiAmbassador_ABC >       ambassador_;
    std::auto_ptr< Federate >                federate_;

    std::auto_ptr< Class< AgentExtension > > agentClass_;
    //@}
};

}

#endif // __FederateFacade_h_
