// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_Fire.h $
// $Author: Jvt $
// $Modtime: 5/04/05 13:34 $
// $Revision: 5 $
// $Workfile: DEC_KS_Fire.h $
//
// *****************************************************************************

#ifndef __DEC_KS_Fire_h_
#define __DEC_KS_Fire_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_KS_Fire
    @brief  This knowledge source (KS) is an 'IN/OUT', which is used to manage
            the fires relatives knowledges
                - direct fire   : detection of the attacker
                - indirect fire : fire information (source / dest) when looking
                                  at a zone where the flying ammos pass through
                - explosion     : explosion information (object concerned)
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_Fire : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_Fire ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_Fire( DEC_KnowledgeBlackBoard& blackBoard, MIL_AgentPion& agentInteracting );
    virtual ~DEC_KS_Fire();
    //@}
    
    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    //@}

    //! @name Events
    //@{
    void NotifyAttackedBy( MIL_AgentPion& attacker );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_AgentPion* >                T_DirectFireAttackerSet;
    typedef T_DirectFireAttackerSet::iterator         IT_DirectFireAttackerSet;
    typedef T_DirectFireAttackerSet::const_iterator   CIT_DirectFireAttackerSet;
    //@}

private:
    MIL_AgentPion*              pAgentInteracting_;
    T_DirectFireAttackerSet     directFireAttackerSet_;
};

#include "DEC_KS_Fire.inl"

#endif // __DEC_KS_Fire_h_
