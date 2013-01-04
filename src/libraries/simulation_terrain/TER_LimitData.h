// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_LimitData_h_
#define __TER_LimitData_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/shared_ptr.hpp>

class TER_DynamicData;
class TER_LimitDataManager;

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class TER_LimitData : private boost::noncopyable
{
    friend class TER_LimitDataManager;

public:
    //! @name
    //@{
    class DistanceData
    {
    public:
        DistanceData( const MT_Vector2D& from, const MT_Vector2D& to );

        double SquareDistance( const MT_Vector2D& p ) const;
        double SquareLength() const;

    private:
        MT_Vector2D origin_;
        MT_Vector2D direction_;
        double rSquareLength_;
    };
    typedef std::vector< DistanceData >     T_DistancesData;
    typedef T_DistancesData::const_iterator CIT_DistancesData;
    //@}

private:
             TER_LimitData( const T_PointVector& points );
public:
    virtual ~TER_LimitData();

    //! @name Accessors
    //@{
    const T_PointVector& GetPoints() const;
    const T_DistancesData& GetDistances() const;
    //@}

    //! @name Geometry - $$$ A GICLER
    //@{
    double GetLength() const;

    double Distance( const MT_Vector2D& p ) const;
    //@}

private:
    //! @name Tools
    //@{
    double SquareDistance         ( const MT_Vector2D& p ) const;
    //@}

private:
    T_DistancesData  distancesData_;
    boost::shared_ptr< TER_DynamicData > data_;
};

#endif // __TER_LimitData_h_
