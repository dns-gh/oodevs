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

#include <tools/Set.h>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>
#include <algorithm>

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
class MIL_ObjectInteraction : private boost::noncopyable
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
    void Clean( MIL_Object_ABC& object );
    void UpdateInteraction( MIL_Object_ABC& object, const TER_Localisation& location );
    void ClearInteraction( MIL_Object_ABC& object );
    void ProcessInteractionEvents( MIL_Object_ABC& object );

    void NotifyPopulationMovingInside( MIL_PopulationElement_ABC& population );
    void NotifyPopulationMovingOutside( MIL_PopulationElement_ABC& population );

    void NotifyAgentMovingInside( MIL_Agent_ABC& agent );
    void NotifyAgentMovingOutside( MIL_Agent_ABC& agent );
    void NotifyAgentPutInside( MIL_Agent_ABC& agent );
    void NotifyAgentPutOutside( MIL_Agent_ABC& agent );

    template< typename F >
    void ProcessInteraction( const F& functor ) const
    {
        std::for_each( agentsInside_.begin(), agentsInside_.end(), functor );
    }
    //@}

private:
    //! @name Types
    //@{
    typedef tools::Set< MIL_Agent_ABC* > T_Agents;
    typedef tools::Set< MIL_PopulationElement_ABC* > T_Populations;
    //@}

private:
    void UpdateAgents( MIL_Object_ABC& object, const TER_Localisation& location, double precision );
    void UpdatePopulations( const TER_Localisation& location, double precision );

private:
    //! @name Member data
    //@{
    T_Agents agentsInside_;
    T_Agents agentsEntering_;
    T_Agents agentsExiting_;
    T_Agents agentsMovingInside_;
    T_Agents agentsDelayedEntering_;
    T_Populations populationsInside_;
    T_Populations populationsMovingInside_;
    double height_;
    //@}
};

#endif // __MIL_ObjectInteraction_h_
