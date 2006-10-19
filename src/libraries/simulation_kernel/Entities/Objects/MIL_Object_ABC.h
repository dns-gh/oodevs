//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Object_ABC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_Object_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_Object_ABC_h_
#define __MIL_Object_ABC_h_

#include "MIL.h"

#include "simulation_terrain/TER_Object_ABC.h"

class MIL_Agent_ABC;
class MIL_Army;
class MIL_Population;
class TER_Localisation;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-15
//=============================================================================
class MIL_Object_ABC : public TER_Object_ABC
{
    MT_COPYNOTALLOWED( MIL_Object_ABC ); 
   
public:
             MIL_Object_ABC();
    virtual ~MIL_Object_ABC();
    
    //! @name Init
    //@{
    virtual void Initialize( const MIL_Army& army, const TER_Localisation& localisation );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateState       ();
            void ProcessEvents     ();
            void MarkForDestruction();
    //@}

    //! @name Events
    //@{
    virtual bool CanInteractWith( const MIL_Agent_ABC& agent       ) const;
    virtual bool CanInteractWith( const MIL_Population& population ) const;

    void NotifyAgentMovingInside ( MIL_Agent_ABC& agent );
    void NotifyAgentMovingOutside( MIL_Agent_ABC& agent );
    void NotifyAgentPutInside    ( MIL_Agent_ABC& agent );
    void NotifyAgentPutOutside   ( MIL_Agent_ABC& agent );
    //@}   

    //! @name Accessors
    //@{
          bool      IsMarkedForDestruction() const;
          bool      IsReadyForDeletion    () const;
    const MIL_Army& GetArmy               () const;

    virtual bool IsReal() const = 0;
    //@}

protected:
    //! @name Types
    //@{
    typedef std::set< MIL_Agent_ABC* >  T_AgentSet;
    typedef T_AgentSet::const_iterator  CIT_AgentSet;
    //@}

protected:
    //! @name Events
    //@{
    virtual void ProcessAgentEntering    ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting     ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside      ( MIL_Agent_ABC& agent );
    //@}

    //! @name Tools
    //@{
    virtual bool      CanCollideWithEntity() const;
    const T_AgentSet& GetAgentsInside     () const;
    virtual void      UpdateLocalisation  ( const TER_Localisation& newLocalisation ); 
    //@}

private:
    const MIL_Army* pArmy_;
          bool      bMarkedForDestruction_;
          bool      bReadyForDeletion_;

    // Link with agents
    T_AgentSet agentInsideSet_;
    T_AgentSet agentEnteringSet_;
    T_AgentSet agentExitingSet_;
    T_AgentSet agentMovingInsideSet_;
};

#include "MIL_Object_ABC.inl"

#endif // __MIL_Object_ABC_h_

