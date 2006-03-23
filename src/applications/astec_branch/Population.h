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
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"

class PopulationPart_ABC;
class PopulationConcentration;
class PopulationFlow;
class Team;
class Controller;
class PopulationType;
class CoordinateConverter;

// =============================================================================
// Created: HME 2005-09-29
// =============================================================================
class Population : public Agent_ABC
                 , public Resolver< PopulationPart_ABC >
                 , private Extension_ABC
                 , public Updatable_ABC< ASN1T_MsgPopulationFluxCreation > // $$$$ AGE 2006-03-13: dégager dans des extensions ?
                 , public Updatable_ABC< ASN1T_MsgPopulationFluxUpdate >
                 , public Updatable_ABC< ASN1T_MsgPopulationFluxDestruction >
                 , public Updatable_ABC< ASN1T_MsgPopulationUpdate >       
                 , public Updatable_ABC< ASN1T_MsgPopulationConcentrationCreation >
                 , public Updatable_ABC< ASN1T_MsgPopulationConcentrationUpdate > 
                 , public Updatable_ABC< ASN1T_MsgPopulationConcentrationDestruction >
                 , public Drawable_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Population( const ASN1T_MsgPopulationCreation& asnMsg, Controller& controller, const CoordinateConverter& converter,
                         const Resolver_ABC< Team >& teamResolver, const Resolver_ABC< PopulationType >& typeResolver );
    virtual ~Population();
    //@}

    //! @name Network
    //@{
    const PopulationConcentration* FindConcentration ( uint nID ) const;
    const PopulationFlow*          FindFlow          ( uint nID ) const;

    const PopulationConcentration& GetConcentration ( uint nID ) const;
    const PopulationFlow&          GetFlow          ( uint nID ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
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

    void DoUpdate( const ASN1T_MsgPopulationFluxCreation&             message );
    void DoUpdate( const ASN1T_MsgPopulationFluxUpdate&               message );
    void DoUpdate( const ASN1T_MsgPopulationFluxDestruction&          message );
    void DoUpdate( const ASN1T_MsgPopulationUpdate&                   message );
    void DoUpdate( const ASN1T_MsgPopulationConcentrationCreation&    message );
	void DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate&      message );
	void DoUpdate( const ASN1T_MsgPopulationConcentrationDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller&                controller_;
    const CoordinateConverter& converter_;
	MIL_AgentID                nPopulationID_;
	std::string                strName_;
    const PopulationType&      type_;
	Team&                      team_;
    //@}

private:
    static MIL_AgentID        nMaxId_;
};

#endif // __Population_h_
