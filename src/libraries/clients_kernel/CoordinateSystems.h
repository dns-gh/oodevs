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

#pragma warning( push )
#pragma warning( disable : 4512 )
#include <boost/ptr_container/ptr_map.hpp>
#pragma warning( pop )

namespace kernel
{
// =============================================================================
/** @class  CoordinateSystems
    @brief  CoordinateSystems
*/
// Created: AME 2010-03-12
// =============================================================================
class CoordinateSystems
{
public:

    typedef boost::ptr_map< int, std::string > T_spatialReference;
    typedef T_spatialReference::const_iterator CIT_spatialReference;

    //! @name Constructors/Destructor
    //@{
             CoordinateSystems();
    virtual ~CoordinateSystems();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CoordinateSystems( const CoordinateSystems& );            //!< Copy constructor
    CoordinateSystems& operator=( const CoordinateSystems& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

public:
     //! @name Member data
    //@{
    enum paramProj
    {
        E_Mgrs,
        E_Wgs84Dd,
        E_Wgs84Dms,
           E_Local
    };

    int defaultCoordinateSystem_;
    T_spatialReference systems_;
    //@}

};
}
#endif // __CoordinateSystems_h_
