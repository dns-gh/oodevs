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

class MIL_DynamicPathDataHandler
{     // Handle Pahthind dynamic data
public:
    //! @name Constructor/destructor
    //@{
    MIL_DynamicPathDataHandler();
    ~MIL_DynamicPathDataHandler();
    //@}

    //! @name 
    //@{
    void Reset( TER_DynamicData* data );
    //@}

private:
    //! @name 
    //@{
    TER_DynamicData*    pPathfindData_;
    //@}
};

#endif // __MIL_DynamicPathDataHandler_h_