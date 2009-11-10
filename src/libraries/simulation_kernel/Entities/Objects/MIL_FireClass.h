// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FireClass_h_
#define __MIL_FireClass_h_

#include "MIL.h"
#include <map>

namespace xml
{
    class xistream;
}

class PHY_Weapon;

// =============================================================================
// Created: RFT 22/04/2008
// MIL_FireClass.h
// Modified: RFT 19/05/2008
// =============================================================================

class MIL_FireClass
{
public:	
    enum E_FireExtinguisherAgent {
        eEauEnMasse = 0,
        eEauEnPulverisation,
        eMousses,
        ePoudres,
        eCO2,
        eSolide,
        eHydrocarburesHalogenes
    };

    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static const MIL_FireClass* Find( const std::string& strName );
    static const MIL_FireClass* Find( uint nID );
    //@}

private:
	//!@ Constructor and destructor
	//@{   
    MIL_FireClass( const std::string& strName, xml::xistream& xis ); 
    ~MIL_FireClass();
	//@}

public:
    //! @name
    //@{
    unsigned int            GetID() const;
    const std::string&      GetName() const;
    int                     GetDefaultHeat() const;
    int                     GetPropagationThreshold() const;
    static unsigned int     GetWidth();
    static unsigned int     GetLength();
    //@}

    struct T_EvaluationResult
    {
        T_EvaluationResult() : agent_( eEauEnMasse ), score_( -1 ), range_( -1 ) {}
        E_FireExtinguisherAgent agent_;
        int score_; // Extinguisher agent effect on a specific fire
        int range_; // fire hose range
    };

    //! @name Operations
    //@{
    int                  ComputeHeatEvolution( int heat, unsigned int timeOfCreation, unsigned int lastTimeSinceUpdate ) const;
    int                  ComputeHeatWhenExtinguishing( int heat, E_FireExtinguisherAgent extinguisherAgent, int numberOfFireHoses ) const;	
    T_EvaluationResult   Evaluate( const PHY_Weapon& weapon ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_FireClass*, sCaseInsensitiveLess > T_FireClassMap;
    typedef T_FireClassMap::const_iterator                                      CIT_FireClassMap;
    
    //The following map contains a extinguisher agent name and its effect on the fire
    typedef std::map< E_FireExtinguisherAgent, int >                            T_ExtinguisherAgentEffectMap;
    typedef T_ExtinguisherAgentEffectMap::const_iterator                        CIT_ExtinguisherAgentEffectMap;
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadClass( xml::xistream& xis );    
           void ReadExtinguisherAgentEffect( xml::xistream& xis );
    //@}

private:
    const    std::string            strName_;
    unsigned int                    nID_;
    unsigned int                    tempThreshold_;
             int                    defaultHeat_;
             int                    heatMax_;
    unsigned int                    increaseRate_;
    unsigned int                    decreaseRate_;
    T_ExtinguisherAgentEffectMap    extinguisherAgentEffect_;
    unsigned int                    propagationThreshold_;
    
    static T_FireClassMap           classes_;
    static unsigned int             length_;
    static unsigned int             width_;
};

#endif // __MIL_FireClass_h_