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

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class TER_LimitData : private boost::noncopyable
{
public:
    virtual ~TER_LimitData();

    //! @name Accessors
    //@{
    const T_PointVector& GetPoints() const;
    const std::vector< double >& GetDistances() const;
    //@}

    //! @name Geometry - $$$ A GICLER
    //@{
    double GetLength() const;

    double Distance( const MT_Vector2D& p ) const;
    //@}

private:
    friend class TER_LimitDataManager;

    TER_LimitData( const T_PointVector& points );

private:
    //! @name Tools
    //@{
    double SquareDistance( const MT_Vector2D& p ) const;
    //@}

private:
    std::vector< double > distancesData_;
    boost::shared_ptr< TER_DynamicData > data_;
};

#endif // __TER_LimitData_h_
