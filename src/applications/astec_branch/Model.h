// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "ASN_Types.h"
#include "Resolver_ABC.h"

class Agent;
class Agent_ABC;
class AgentFactory_ABC;
class ObjectFactory_ABC;
class Team;
class LogMaintenanceConsign;
class LogSupplyConsign;
class LogMedicalConsign;
class Gtia;
class Object_ABC;
class App;
class Population;
class TacticalLine_ABC;

namespace DIN {
    class DIN_Input;
};


// =============================================================================
/** @class  Model
    @brief  Model

    // $$$$ AGE 2006-02-14: revoir 
*/
// Created: AGE 2006-02-10
// =============================================================================
class Model : public Resolver_ABC< Team >
            , public Resolver_ABC< Gtia > 
            , public Resolver_ABC< Agent > // $$$$ AGE 2006-02-13: eventualy Agent_ABC
            , public Resolver_ABC< Object_ABC >
            , public Resolver_ABC< LogMaintenanceConsign >
            , public Resolver_ABC< LogMedicalConsign >
            , public Resolver_ABC< LogSupplyConsign >
            , public Resolver_ABC< Population >
            , public Resolver_ABC< TacticalLine_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Model( AgentFactory_ABC& agentFactory, ObjectFactory_ABC& objectFactory ); // $$$$ AGE 2006-02-10: eventually a controller
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void CreateTeam( DIN::DIN_Input& );
    Team& GetTeam( unsigned long id );
    Team* FindTeam( const std::string& team );
    template< typename Functor >
    void ApplyOnTeams( const Functor& f ) {
        Resolver_ABC< Team >::Apply( f );
    }


    void CreateGtia( DIN::DIN_Input& );
    Gtia& GetGtia( unsigned long id );

    void CreateAgent( const ASN1T_MsgAutomateCreation& asnMsg );
    void CreateAgent( const ASN1T_MsgPionCreation& asnMsg );
    Agent& GetAgent( unsigned long id );
    Agent* FindAgent( unsigned long id ); // $$$$ AGE 2006-02-13: 

    template< typename Functor >
    void ApplyOnAgents( const Functor& f ) {
        Resolver_ABC< Agent >::Apply( f );
    }

    Agent_ABC* FindAllAgent( unsigned long id ); // $$$$ AGE 2006-02-13: 

    void CreatePopulation( const ASN1T_MsgPopulationCreation& asnMsg );
    Population& GetPopulation( unsigned long id );
    Population* FindPopulation( unsigned long id );
    
    void CreateObject( const ASN1T_MsgObjectCreation& asnMsg );
    Object_ABC& GetObject( unsigned long id );
    void DeleteObject( unsigned long id );

    void CreateMaintenanceConsign( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg );
    LogMaintenanceConsign& GetMaintenanceConsign( unsigned long id );
    void DeleteMaintenanceConsign( unsigned long id );

    void CreateMedicalConsign( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg );
    LogMedicalConsign& GetMedicalConsign( unsigned long id );
    void DeleteMedicalConsign( unsigned long id );

    void CreateSupplyConsign( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg );
    LogSupplyConsign& GetSupplyConsign( unsigned long id );
    void DeleteSupplyConsign( unsigned long id );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    template< typename Type >
    void Delete( unsigned long id );
    //@}

private:
    //! @name Member data
    //@{
    AgentFactory_ABC& agentFactory_;
    ObjectFactory_ABC& objectFactory_;
    //@}
};

#endif // __Model_h_
