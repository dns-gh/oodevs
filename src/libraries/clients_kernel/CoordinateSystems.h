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
#include <QString>
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
    //! @name Projection
    //@{
    enum Projection
    {
        E_Mgrs,
        E_SanC,
        E_Wgs84Dd,
        E_Wgs84Dms,
        E_Local
    };
    static QString    Convert( Projection proj );
    static Projection Convert( const QString& proj );
    //@}

    //! @name Types
    //@{
    typedef std::map< int, QString > T_SpatialReference;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    CoordinateSystems();
    //@}

    //! @name Setters/Getters
    //@{
    void SetDefault( Projection projection );
    const Projection GetDefault() const;

    const std::map< int, QString >& GetSystems() const;
    //@}

private:
     //! @name Member data
    //@{
    Projection default_;
    const std::map< int, QString > systems_;
    //@}
};

}
#endif // __CoordinateSystems_h_
