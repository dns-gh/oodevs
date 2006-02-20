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

#include "Types.h"
#include "ASN_Types.h"

#include "Agent_ABC.h"
#include "AgentKnowledge.h"
#include "Resolver.h"

class PopulationPart_ABC;
class PopulationConcentration;
class PopulationFlow;
class Team;
class Controller;
class PopulationType;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public Agent_ABC
                 , public Resolver< PopulationPart_ABC >
{
public:
    //! @name Constructor/Destructor
    //@{
             Population( const ASN1T_MsgPopulationCreation& asnMsg, Controller& controller, 
                         const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< PopulationType >& typeResolver );
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

    const PopulationConcentration* FindConcentration ( uint nID ) const;
    const PopulationFlow*          FindFlow          ( uint nID ) const;

    const PopulationConcentration& GetConcentration ( uint nID ) const;
    const PopulationFlow&          GetFlow          ( uint nID ) const;

    //! @name Operations
    //@{
    unsigned int GetLivingHumans() const;
    unsigned int GetDeadHumans() const;
    //@}

    //! @name Accessors
    //@{
    const Team& GetTeam() const;
    virtual unsigned long     GetId() const;
    virtual std::string       GetName() const;
    //@}
    
private:
    //! @name Copy / Assignment
    //@{
    Population( const Population& );
    Population& operator=( const Population& );
    //@}

    //! @name Helpers
    //@{
    unsigned int ComputeLivingHumans() const;
    unsigned int ComputeDeadHumans() const;
    //@}

private:
    //! @name Member data
    //@{
    Controller&           controller_;
	MIL_AgentID           nPopulationID_;
	std::string           strName_;
    const PopulationType& type_;
	Team&                 team_;
    //@}

private:
    static MIL_AgentID        nMaxId_;
};

#endif // __Population_h_
