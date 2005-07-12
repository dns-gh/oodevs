// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_NetworkUpdater.h $
// $Author: Jvt $
// $Modtime: 23/03/05 18:58 $
// $Revision: 3 $
// $Workfile: DEC_KS_NetworkUpdater.h $
//
// *****************************************************************************

#ifndef __DEC_KS_NetworkUpdater_h_
#define __DEC_KS_NetworkUpdater_h_

#include "MIL.h"

#include "DEC_KnowledgeSource_ABC.h"

class DEC_KnowledgeBlackBoard;
class NET_AS_MOSServer;

// =============================================================================
/** @class  DEC_KS_NetworkUpdater
    @brief  This knowledge source (KS) is an 'OUT', which is used to send the 
            knowledges on the network
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_KS_NetworkUpdater : public DEC_KnowledgeSource_ABC
{
    MT_COPYNOTALLOWED( DEC_KS_NetworkUpdater ) 
public:
    //! @name Constructors/Destructor
    //@{
             DEC_KS_NetworkUpdater( DEC_KnowledgeBlackBoard& blackBoard );
    virtual ~DEC_KS_NetworkUpdater();
    //@}

    //! @name Queries / Operations
    //@{
    void SendFullState();
    //@}

    //! @name Operations
    //@{
    virtual void Prepare();
    virtual void Talk   ();
    //@}
};

#include "DEC_KS_NetworkUpdater.inl"

#endif // __DEC_KS_NetworkUpdater_h_
