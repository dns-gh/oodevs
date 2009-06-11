// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_DynamicPathDataHandler_h_
#define __MIL_DynamicPathDataHandler_h_

class TER_DynamicData;

// =============================================================================
/** @class  MIL_DynamicPathDataHandler
    @brief  Handle Pahthind dynamic data
*/
// Created: JCR 2008-06-02
// =============================================================================
class MIL_DynamicPathDataHandler
{
public:
    //! @name Constructor/destructor
    //@{
     MIL_DynamicPathDataHandler();
    ~MIL_DynamicPathDataHandler();
    //@}

    //! @name Operations
    //@{
    void Reset( TER_DynamicData* data );
    //@}

private:
    //! @name Member data
    //@{
    TER_DynamicData* pPathfindData_;
    //@}
};

#endif // __MIL_DynamicPathDataHandler_h_