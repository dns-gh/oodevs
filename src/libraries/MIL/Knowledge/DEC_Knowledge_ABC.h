// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/knowledge/DEC_Knowledge_ABC.h $
// $Author: Jvt $
// $Modtime: 25/03/05 13:13 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ABC_h_
#define __DEC_Knowledge_ABC_h_

#include "MIL.h"

#include "DEC_Knowledge_Def.h"

// =============================================================================
/** @class  DEC_Knowledge_ABC
    @brief  The base class for a knowledge
*/
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_ABC
{
	MT_COPYNOTALLOWED( DEC_Knowledge_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ABC();
    virtual ~DEC_Knowledge_ABC();
    //@}
    
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}
    
protected:
    //! @name Tools
    //@{
    uint GetCurrentTimeStep() const; 
    //@}
};

#include "DEC_Knowledge_ABC.inl"

#endif // __DEC_Knowledge_ABC_h_
