// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __SpatialRequestStatus_h_
#define __SpatialRequestStatus_h_

namespace geostore
{

// =============================================================================
/** @class  SpatialRequestStatus
    @brief  SpatialRequestStatus
*/
// Created: AME 2010-08-02
// =============================================================================
class SpatialRequestStatus : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SpatialRequestStatus();
    virtual ~SpatialRequestStatus();
    //@}

    //! @name Operations
    //@{
    void UpdateSpatialStatus( int number, bool status );
    //@}

public:
    //! @name Member data
    //@{
    int  numberCreatedObjects_;
    bool valid_;
    //@}
};

} //! namespace geostore

#endif // __SpatialRequestStatus_h_
