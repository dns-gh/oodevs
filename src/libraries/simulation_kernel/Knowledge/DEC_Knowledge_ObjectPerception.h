// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectPerception.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_ObjectPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectPerception_h_
#define __DEC_Knowledge_ObjectPerception_h_

#include "MIL.h"
#include "DEC_Knowledge_ABC.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"

class MIL_Object_ABC;
class MIL_AgentPion;

// =============================================================================
/** @class  DEC_Knowledge_ObjectPerception
    @brief  This class represents the perception of an object 
            An object can be
                - perceived : the object is seen by the sensors
                - detected  : the object 
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_ObjectPerception : public DEC_Knowledge_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectPerception( const MIL_AgentPion& agentPerceiving, MIL_Object_ABC& objectPerceived );
             DEC_Knowledge_ObjectPerception();
    virtual ~DEC_Knowledge_ObjectPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare();
    void Update ( const PHY_PerceptionLevel& perceptionLevel );
    bool Clean  (); 
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

    //! @name Accessors
    //@{
          MIL_Object_ABC&      GetObjectPerceived       () const;
    const MIL_AgentPion&       GetAgentPerceiving       () const;
    const PHY_PerceptionLevel& GetCurrentPerceptionLevel() const;
          bool                 IsPerceived              () const;
    //@}

private:
    const MIL_AgentPion*       pAgentPerceiving_;
          MIL_Object_ABC*      pObjectPerceived_;

    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
};

#endif // __DEC_Knowledge_ObjectPerception_h_
