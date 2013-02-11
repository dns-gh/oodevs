// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CoordinateSystems_h_
#define __CoordinateSystems_h_

#include <boost/noncopyable.hpp>
#include <map>

namespace kernel
{
// =============================================================================
/** @class  CoordinateSystems
    @brief  CoordinateSystems
*/
// Created: AME 2010-03-12
// =============================================================================
class CoordinateSystems : public boost::noncopyable
{
public:
    enum Projection
    {
        E_Mgrs,
        E_Wgs84Dd,
        E_Wgs84Dms,
        E_Local
    };

    typedef std::map< int, QString > T_SpatialReference;

    //! @name Constructors/Destructor
    //@{
             CoordinateSystems();
    virtual ~CoordinateSystems();
    //@}

public:
     //! @name Member data
    //@{
    int default_;
    const T_SpatialReference systems_;
    //@}

};
}
#endif // __CoordinateSystems_h_
