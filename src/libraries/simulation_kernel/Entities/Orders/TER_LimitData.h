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

class TER_DynamicData;
class MIL_Fuseau;

//=============================================================================
// Created: NLD 2002-08-06
// Last modified: JVT 03-02-27
//=============================================================================
class TER_LimitData : private boost::noncopyable
{

public:
     TER_LimitData( const T_PointVector& points );
    ~TER_LimitData();

    //! @name Accessors
    //@{
    const T_PointVector& GetPoints() const;
    //@}

    //! @name Operations
    //@{
    void AddRef( const MIL_Fuseau& fuseau ) const;
    void DecRef( const MIL_Fuseau& fuseau ) const;
    //@}

    //! @name Geometry - $$$ A GICLER
    //@{
    MT_Float GetLength() const;

    MT_Float Distance( const MT_Vector2D& p ) const;
    //@}

private:
    //! @name 
    //@{
    class DistanceData
    {
    public:
        DistanceData( const MT_Vector2D& from, const MT_Vector2D& to );
        MT_Float SquareDistance( const MT_Vector2D& p ) const;
    private:
        MT_Vector2D origin_;
        MT_Vector2D direction_;
        MT_Float    rSquareLength_;
    };
    typedef std::vector< DistanceData >     T_DistancesData;
    typedef T_DistancesData::const_iterator CIT_DistancesData;
    //@}

private:
    //! @name Tools
    //@{
    void     InitializeDistancesData();
    MT_Float SquareDistance         ( const MT_Vector2D& p ) const;
    //@}

private:
    const T_PointVector    points_;
          TER_DynamicData* pPathFindData_;
          T_DistancesData  distancesData_;
    mutable uint           nNbRefs_;
};

#endif // __TER_LimitData_h_
