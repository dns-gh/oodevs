// *****************************************************************************
//
// $Created: NLD 2004-06-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Debug.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: DEC_Debug.h $
//
// *****************************************************************************

#ifndef __DEC_Debug_h_
#define __DEC_Debug_h_

#include "MIL.h"

class PHY_Action_ABC;
class MIL_AgentPion;
class MIL_Population;

// =============================================================================
// Created: NLD 2004-06-04
// =============================================================================
class DEC_Debug : public DIA_InstanceEndHandler_ABC
{
public:
    //! @name Constructor / destructor
    //@{
     DEC_Debug();
    ~DEC_Debug();
    //@}

    //! @name DIA Events
    //@{
    virtual void HandlesEnd( const DIA_Instance* pInstance );
    //@}

    //! @name Events
    //@{
    static void NotifyRecoAdded    ( DIA_Call_ABC& call, const MT_Vector2D& vCenter );
    static void NotifyRecoRemoved  ( DIA_Call_ABC& call, const MT_Vector2D& vCenter );
    static void NotifyActionStarted( DIA_Call_ABC& call, const MIL_AgentPion&  pion      , const PHY_Action_ABC& action );
    static void NotifyActionStarted( DIA_Call_ABC& call, const MIL_Population& population, const PHY_Action_ABC& action );
    static void NotifyActionStopped( DIA_Call_ABC& call, const MIL_AgentPion&  pion      , const PHY_Action_ABC& action );
    static void NotifyActionStopped( DIA_Call_ABC& call, const MIL_Population& population, const PHY_Action_ABC& action );
    //@}

private:
    //! @name Types
    //@{
    typedef std::multimap< const void*, MT_Vector2D /*reco center*/ > T_RecoPerInstanceMap;
    typedef T_RecoPerInstanceMap::iterator                            IT_RecoPerInstanceMap;
    typedef T_RecoPerInstanceMap::const_iterator                      CIT_RecoPerInstanceMap;
    typedef std::pair< IT_RecoPerInstanceMap, IT_RecoPerInstanceMap > T_RangeRecoPerInstanceMap;
    
    typedef std::multimap< const void*, const PHY_Action_ABC* >             T_ActionPerInstanceMap;
    typedef T_ActionPerInstanceMap::iterator                                IT_ActionPerInstanceMap;
    typedef T_ActionPerInstanceMap::const_iterator                          CIT_ActionPerInstanceMap;
    typedef std::pair< IT_ActionPerInstanceMap, IT_ActionPerInstanceMap >   T_RangeActionPerInstanceMap;
    //@} 

private:
    //! @name DIA Callbacks
    //@{
    static void DIAAssert( const std::string& strTestExpression, const std::string& strFileName, int nLine, const std::string& strMore );
    //@}

private:
    //! @name Member data
    //@{
    static T_RecoPerInstanceMap     recoPerInstanceMap_;
    static T_ActionPerInstanceMap   actionPerInstanceMap_;
    //@}
};

#include "DEC_Debug.inl"

#endif // __DEC_Debug_h_
