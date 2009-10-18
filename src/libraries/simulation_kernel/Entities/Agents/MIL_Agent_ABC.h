// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_Agent_ABC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_Agent_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_Agent_ABC_h_
#define __MIL_Agent_ABC_h_

#include "simulation_kernel/Entities/MIL_Entity_ABC.h"

class MIL_Army_ABC;
class MIL_KnowledgeGroup;
class MIL_AgentType_ABC;
class MIL_AgentPion;
class MIL_Population;
class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_KnowledgeBlackBoard_AgentPion;
class AlgorithmsFactories;

// =============================================================================
// @class  MIL_Agent_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Agent_ABC : public MIL_Entity_ABC
{

public:
             MIL_Agent_ABC( xml::xistream& xis, uint nID );
             MIL_Agent_ABC( const std::string& name, uint nID );
    virtual ~MIL_Agent_ABC();

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Accessors
    //@{
            uint                      GetID        () const;
    virtual       MIL_Army_ABC&       GetArmy      () const = 0;
    virtual const MIL_AgentType_ABC&  GetType      () const = 0;
    virtual bool                      IsDead       () const = 0;
    virtual bool                      IsNeutralized() const = 0;
    virtual bool                      IsPC         () const = 0;
    
    virtual       DEC_Decision_ABC& GetDecision() = 0;
    virtual const DEC_Decision_ABC& GetDecision() const = 0;

    virtual const AlgorithmsFactories& GetAlgorithms() const = 0;

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void NotifyAttackedBy( MIL_AgentPion& pion ) = 0;
    virtual void NotifyAttackedBy( MIL_Population& population ) = 0;
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Agent& CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup ) = 0;
    virtual bool                 IsPerceived    ( const MIL_Agent_ABC& target ) const = 0; //$$$$ DEGUEU - VOIR AVEC MODELISATEURS
    virtual DEC_KnowledgeBlackBoard_AgentPion& GetKnowledge   () const = 0; 
    //@}

    //! @name Operators
    //@{
    bool operator ==( const MIL_Agent_ABC& rhs ) const;
    bool operator !=( const MIL_Agent_ABC& rhs ) const;
    //@}

private:
    uint nID_;
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void MIL_Agent_ABC::serialize
// Created: SBO 2009-07-01
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_Agent_ABC::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_Entity_ABC >( *this );
    file & nID_;
}

#endif // __MIL_Agent_ABC_h_
