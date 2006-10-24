//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_SiteDecontamination.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 4 $
// $Workfile: MIL_SiteDecontamination.h $
//
//*****************************************************************************

#ifndef __MIL_SiteDecontamination_h_
#define __MIL_SiteDecontamination_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_AgentPion;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_SiteDecontamination : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_SiteDecontamination );

public:
             MIL_SiteDecontamination( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_SiteDecontamination();
    virtual ~MIL_SiteDecontamination();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}

    //! @name Operations
    //@{
    void QueueForDecontamination( MIL_Agent_ABC& agent );
    virtual void UpdateState();
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentInside( MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Agent_ABC* > T_AgentQueueSet;
    //@}

private:
    T_AgentQueueSet decontaminationQueue_;
    bool            bHasDecontaminated_;
};

#include "MIL_SiteDecontamination.inl"

#endif // __MIL_SiteDecontamination_h_
