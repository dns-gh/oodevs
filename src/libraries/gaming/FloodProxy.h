// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FloodProxy_h_
#define __FloodProxy_h_

#include "flood/ElevationGetter_ABC.h"

namespace flood
{
    class FloodDrawer;
    class FloodModel;
}

namespace kernel
{
    class DetectionMap;
}

// =============================================================================
/** @class  FloodProxy
    @brief  FloodProxy
*/
// Created: JSR 2011-10-07
// =============================================================================
class FloodProxy : public flood::ElevationGetter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodProxy( const kernel::DetectionMap& detection );
    virtual ~FloodProxy();
    //@}

    //! @name Operations
    //@{
    virtual short GetElevationAt( const geometry::Point2f& point ) const;
    void Draw( unsigned int floodId ) const;
    void Remove( unsigned int floodId );
    unsigned int GenerateFlood( unsigned int floodId, const geometry::Point2f& point, int depth, int refDist );
    unsigned int FindFlood( const geometry::Point2f& point, int depth, int refDist ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< std::auto_ptr< flood::FloodModel >, std::auto_ptr< flood::FloodDrawer > > T_Flood;
    typedef std::map< unsigned int, T_Flood > T_Floods;
    typedef T_Floods::const_iterator        CIT_Floods;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& detection_;
    T_Floods floods_;
    unsigned int idManager_;
    //@}
};

#endif // __FloodProxy_h_
