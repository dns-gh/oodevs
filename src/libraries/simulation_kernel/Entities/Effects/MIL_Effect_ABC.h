// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_ABC.h $
// $Author: Age $
// $Modtime: 29/11/04 14:55 $
// $Revision: 2 $
// $Workfile: MIL_Effect_ABC.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_ABC_h_
#define __MIL_Effect_ABC_h_

// =============================================================================
// @class  MIL_Effect_ABC
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_ABC : private boost::noncopyable
{

public:
             MIL_Effect_ABC() {}
    virtual ~MIL_Effect_ABC() {}

    //! @name Operations
    //@{
    virtual bool Execute() = 0;
    //@}
};

#endif // __MIL_Effect_ABC_h_
