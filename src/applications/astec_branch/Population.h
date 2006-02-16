// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Population_h_
#define __Population_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "ASN_Types.h"

#include "Agent_ABC.h"
#include "TypePopulation.h"
#include "AgentKnowledge.h"
#include "Resolver.h"

class PopulationConcentration;
class PopulationFlow;
class Team;
class Controller;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public Agent_ABC
                 , public Resolver< PopulationConcentration >
                 , public Resolver< PopulationFlow >
{
public:
    //! @name Constructor/Destructor
    //@{
             Population( const ASN1T_MsgPopulationCreation& asnMsg, Controller& controller, const Resolver_ABC< Team >& resolver );
    virtual ~Population();
    //@}

    //! @name Network
    //@{
	void Update( const ASN1T_MsgPopulationFluxCreation&             message );
    void Update( const ASN1T_MsgPopulationFluxUpdate&               message );
    void Update( const ASN1T_MsgPopulationFluxDestruction&          message );
    void Update( const ASN1T_MsgPopulationUpdate&                   message );
    void Update( const ASN1T_MsgPopulationConcentrationCreation&    message );
	void Update( const ASN1T_MsgPopulationConcentrationUpdate&      message );
	void Update( const ASN1T_MsgPopulationConcentrationDestruction& message );
    //@}

    const Team& GetTeam() const;

    const PopulationConcentration* FindConcentration ( uint nID ) const;
    const PopulationFlow*          FindFlow          ( uint nID ) const;

    const PopulationConcentration& GetConcentration ( uint nID ) const;
    const PopulationFlow&          GetFlow          ( uint nID ) const;

    //! @name Accessors
    //@{
    virtual unsigned long     GetId() const;
    virtual std::string       GetName() const;
    //@}
    
private:

    //! @name Copy / Assignment
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
    //@}

private:
    //! @name Member data
    //@{
    Controller&           controller_;
	MIL_AgentID           nPopulationID_;
	std::string           strName_;
    const TypePopulation* pType_;
	Team&                 team_;
    //@}

private:
    static MIL_AgentID        nMaxId_;
};

#endif // __Population_h_
