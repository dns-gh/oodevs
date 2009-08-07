// *****************************************************************************
//
// $Created: AGE 2004-11-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentHLA.h $
// $Author: Jvt $
// $Modtime: 25/03/05 12:39 $
// $Revision: 7 $
// $Workfile: MIL_AgentHLA.h $
//
// *****************************************************************************

#ifndef __MIL_AgentHLA_h_
#define __MIL_AgentHLA_h_

#include "MIL_Agent_ABC.h"

namespace hla
{
    class Deserializer;
    class AttributeIdentifier;
    class ObjectIdentifier;
}

class HLA_InteractionManager_ABC;

// =============================================================================
/** @class  MIL_AgentHLA
    @brief  HLA agent
*/
// Created: AGE 2004-11-09
// =============================================================================
class MIL_AgentHLA : public MIL_Agent_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_AgentHLA( uint nID, const hla::ObjectIdentifier& objectId, HLA_InteractionManager_ABC& interactionManager );
    virtual ~MIL_AgentHLA();
    //@}
        
    //! @name Operations
    //@{
    virtual       MIL_Army_ABC&       GetArmy          () const;
    virtual const MIL_AgentType_ABC&  GetType          () const;
    virtual bool                      IsDead           () const;
    virtual bool                      IsNeutralized    () const;
    virtual bool                      IsPC             () const;

    virtual bool BelongsTo( const MIL_KnowledgeGroup& group ) const;

    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

    //! @name Operations
    //@{
    virtual void NotifyAttackedBy( MIL_AgentPion& pion );
    virtual void NotifyAttackedBy( MIL_Population& population );
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Agent& CreateKnowledge( const MIL_KnowledgeGroup& knowledgeGroup );
    virtual bool                 IsPerceived    ( const MIL_Agent_ABC& agent ) const;
    virtual DEC_KnowledgeBlackBoard_AgentPion& GetKnowledge   () const;
    //@}

    //! @name Accessors
    //@{
    bool IsValid() const;
    
    virtual       DEC_Decision_ABC& GetDecision   ();
    virtual const DEC_Decision_ABC& GetDecision   () const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_AgentHLA( const MIL_AgentHLA& );            //!< Copy constructor
    MIL_AgentHLA& operator=( const MIL_AgentHLA& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
          MIL_Army_ABC*      pArmy_;
    const MIL_AgentType_ABC* pType_;
          bool               bPc_;
    //@}
};

#endif // __MIL_AgentHLA_h_
