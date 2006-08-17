// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_RapForGlobal.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: DEC_Knowledge_RapForGlobal.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_RapForGlobal_h_
#define __DEC_Knowledge_RapForGlobal_h_

#include "MIL.h"

#include "DEC_Knowledge_RapFor_ABC.h"

class MIL_Automate;

// =============================================================================
// Created: NLD 2004-04-07
// =============================================================================
class DEC_Knowledge_RapForGlobal : public DEC_Knowledge_RapFor_ABC
{
    MT_COPYNOTALLOWED( DEC_Knowledge_RapForGlobal );

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_RapForGlobal( const MIL_Automate& automate );
             DEC_Knowledge_RapForGlobal();
    virtual ~DEC_Knowledge_RapForGlobal();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    //@}

private:
    const MIL_Automate* pAutomate_;
};

#include "DEC_Knowledge_RapForGlobal.inl"

#endif // __DEC_Knowledge_RapForGlobal_h_
