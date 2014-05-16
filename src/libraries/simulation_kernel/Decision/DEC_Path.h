//*****************************************************************************
//
// $Created: JDY 03-02-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.h $
// $Author: Age $
// $Modtime: 28/02/05 10:04 $
// $Revision: 2 $
// $Workfile: DEC_Path.h $
//
//*****************************************************************************

#ifndef __DEC_Path_h_
#define __DEC_Path_h_

#include "DEC_Path_ABC.h"

// =============================================================================
// @class  DEC_Path
// Created: NLD 2005-02-22
// =============================================================================
class DEC_Path : public DEC_Path_ABC
{
public:
    //! @name Operations
    //@{
    virtual void Destroy() {}
    //@}

protected:
    //! @name Constructor / Destructor
    //@{
             DEC_Path();
    virtual ~DEC_Path();
    //@}
};

#endif // __DEC_Path_h_
