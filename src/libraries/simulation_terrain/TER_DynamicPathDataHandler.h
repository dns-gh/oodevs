// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef TER_DYNAMICPATHDATAHANDLER_H
#define TER_DYNAMICPATHDATAHANDLER_H

class TER_DynamicData;

class TER_DynamicPathDataHandler
{
public:
    //! @name Constructor/destructor
    //@{
    TER_DynamicPathDataHandler();
    ~TER_DynamicPathDataHandler();
    //@}

    //! @name Operations
    //@{
    void Reset( TER_DynamicData* data );
    //@}

    //! @name Accessors
    //@{
    bool IsPathfindData() const { return pPathfindData_ != 0; }
    //@}

private:
    //! @name Member data
    //@{
    TER_DynamicData* pPathfindData_;
    //@}
};

#endif // TER_DYNAMICPATHDATAHANDLER_H
