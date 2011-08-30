// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ObjectInteraction_h_
#define __MIL_ObjectInteraction_h_

#include <boost/serialization/split_member.hpp>
#include <boost/function.hpp>

class MIL_Agent_ABC;
class TER_Localisation;
class MIL_CheckPointOutArchive;
class MIL_CheckPointInArchive;
class MIL_Object_ABC;
class MIL_PopulationElement_ABC;

// =============================================================================
/** @class  MIL_ObjectInteraction
    @brief  MIL_ObjectInteraction
*/
// Created: JCR 2008-05-26
// =============================================================================
class MIL_ObjectInteraction
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ObjectInteraction();
    virtual ~MIL_ObjectInteraction();
    //@}

    //! @name Check points
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void UpdateInteraction( MIL_Object_ABC& object, const TER_Localisation& location );
    void ClearInteraction( MIL_Object_ABC& object );
    void ProcessInteractionEvents( MIL_Object_ABC& object );
    //@}

    //! @name Operations
    //@{
    void NotifyPopulationMovingInside( MIL_PopulationElement_ABC& population );
    void NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population );

    void NotifyAgentMovingInside( MIL_Agent_ABC& agent );
    void NotifyAgentMovingOutside( MIL_Agent_ABC& agent );
    void NotifyAgentPutInside( MIL_Agent_ABC& agent );
    void NotifyAgentPutOutside( MIL_Agent_ABC& agent );
    //@}

    //! @name Process Agent inside
    //@{
    template< typename T > void ProcessInteraction( T functor ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Agent_ABC* >   T_AgentSet;
    typedef T_AgentSet::const_iterator CIT_AgentSet;

    typedef std::set< MIL_PopulationElement_ABC* > T_PopulationSet;
    typedef T_PopulationSet::const_iterator      CIT_PopulationSet;
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const T_PopulationSet& last, const T_PopulationSet& current, boost::function< void( MIL_PopulationElement_ABC& ) > fun );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ObjectInteraction( const MIL_ObjectInteraction& );            //!< Copy constructor
    MIL_ObjectInteraction& operator=( const MIL_ObjectInteraction& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    // Links with agents
    T_AgentSet agentInsideSet_;
    T_AgentSet agentEnteringSet_;
    T_AgentSet agentExitingSet_;
    T_AgentSet agentMovingInsideSet_;
    T_AgentSet agentDelayedEnteringSet_;
    // link with population
    T_PopulationSet populationInsideSet_;
    double height_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename T > void MIL_ObjectInteraction::ProcessInteraction
// Created: JCR 2008-08-28
// -----------------------------------------------------------------------------
template< typename T >
void MIL_ObjectInteraction::ProcessInteraction( T functor ) const
{
    std::for_each( agentInsideSet_.begin(), agentInsideSet_.end(), functor );
}

#endif // __MIL_ObjectInteraction_h_
