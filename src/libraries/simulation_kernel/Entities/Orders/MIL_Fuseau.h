//
// $Created: NLD 2003-01-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/MIL_Fuseau.h $
// $Author: Age $
// $Modtime: 13/05/05 19:05 $
// $Revision: 5 $
// $Workfile: MIL_Fuseau.h $
//
//*****************************************************************************

#ifndef __MIL_Fuseau_h_
#define __MIL_Fuseau_h_

#include "simulation_terrain/TER_Polygon.h"
#include "MIL.h"

class TER_Localisation;
class TER_LimitData;
class MIL_LimaOrder;
class DEC_Objective;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

//=============================================================================
// Created: NLD 2003-01-14
//=============================================================================
class MIL_Fuseau : public TER_Polygon
{
public:
    //! @name Types
    //@{
    typedef std::list< MIL_Fuseau* >  T_FuseauPtrList;
    typedef T_FuseauPtrList::iterator IT_FuseauPtrList;
    //@}

public:
             MIL_Fuseau();
             MIL_Fuseau( const MT_Vector2D& vOrientationRefPos, const T_PointVector& leftLimit, const T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima = 0, const MIL_LimaOrder* pEndMissionLima = 0 );
    virtual ~MIL_Fuseau();

    //! @name Init
    //@{
    void Reset( const MT_Vector2D& vOrientationRefPos, const T_PointVector& leftLimit, const T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima = 0 );
    void Reset();
    //@}

    //! @name Geometry
    //@{
    bool IsInside           ( const MT_Vector2D& vPos  ) const;
    bool IntersectWithBorder( const MT_Line& line ) const;
    bool IntersectWithBorder( const MT_Line& line, T_PointSet& res ) const;

    bool   ComputeFurthestExtremityPoint      ( MT_Vector2D& vResult ) const;
    bool   ComputeClosestExtremityPoint       ( MT_Vector2D& vResult ) const;
    bool   ComputePointBeforeLima             ( const MIL_LimaOrder& lima, double rDistBefore, MT_Vector2D&   vResult ) const;
    bool   ComputePointsBeforeLima            ( const MIL_LimaOrder& lima, double rDistBefore, unsigned int nNbPoints, T_PointVector& results ) const;
    void   ComputeNearestEntryPoint           ( const MT_Vector2D& vStartPos, MT_Vector2D& vResult ) const;
    void   ComputeEntryPoint                  ( const MT_Vector2D& vStartPos, MT_Vector2D& vResult ) const;
    double ComputeAverageDistanceFromLima     ( const MIL_LimaOrder& lima     , const MT_Vector2D& refPoint ) const;
    double ComputeAverageDistanceFromObjective( const DEC_Objective& objective, const MT_Vector2D& refPoint ) const;
    double ComputeAdvance                     ( const MT_Vector2D& position ) const;

    float ComputeOpenTerrainRatio  () const;
    float ComputeClosedTerrainRatio() const;

    bool IsLeftFlank ( const MIL_Fuseau& fuseau ) const;
    bool IsRightFlank( const MIL_Fuseau& fuseau ) const;
    bool IsOnFlank   ( const MT_Vector2D& position, bool left, bool right ) const;
    //@}

    //! @name Pathfind
    //@{
    double Distance( const MT_Vector2D& p, bool bLimitsOnly = false ) const;
    double GetCost ( const MT_Vector2D&, const MT_Vector2D& to, double rMaxDistanceOut, double rCostPerMeterOut, double rComfortDistanceIn, double rCostPerMeterIn ) const;
    //@}

    //! @name Accessors
    //@{
    const TER_LimitData* GetLeftLimit () const;
    const TER_LimitData* GetRightLimit() const;

    bool             IsNull            () const;
    const MT_Line&   GetGlobalDirection() const;
    double           GetWidth          () const;
    //@}

    //! @name Tools
    //@{
    bool SplitIntoSubFuseaux( unsigned int nNbrSubFuseau, T_FuseauPtrList& container ) const;
    //@}

    //! @name Operators
    //@{
    MIL_Fuseau& operator= ( const MIL_Fuseau& );
    bool        operator==( const MIL_Fuseau& rhs ) const;
    bool        operator!=( const MIL_Fuseau& rhs ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< T_PointVector >        T_PointVectorVector;
    typedef T_PointVectorVector::iterator       IT_PointVectorVector;
    typedef T_PointVectorVector::const_iterator CIT_PointVectorVector;
    //@}

private:
     MIL_Fuseau( const MIL_Fuseau& rhs ); // ????

    //! @name Init
    //@{
    void InitializePolygon        ();
    void TruncateAndReorientLimits( T_PointVector& leftLimit, T_PointVector& rightLimit, const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima );
    void SplitLimit               ( const MIL_LimaOrder* pBeginMissionLima, const MIL_LimaOrder* pEndMissionLima, const T_PointVector& limit, T_PointVectorVector& parts ) const;
    bool IsPointInsidePolygon     ( T_PointVector& leftPoints, T_PointVector& rightPoints, const MT_Vector2D& vPoint );
    void InitializeMiddleLimit    ();
    //@}

    //! @name Tools
    //@{
    bool Split( unsigned int nNbrSubFuseau, T_PointVectorVector& limitVector ) const;
    //@}

private:
    const TER_LimitData* pLeftLimit_;
    const TER_LimitData* pRightLimit_;
    const TER_LimitData* pMiddleLimit_;

    // Fuseau global direction
    MT_Vector2D vOrientationRefPos_;
    MT_Vector2D vStartGlobalDirection_;
    MT_Vector2D vEndGlobalDirection_;
    MT_Line globalDirectionLine_;

private:
    static unsigned int nNbrMeterPerSample_;
};

BOOST_CLASS_EXPORT_KEY( MIL_Fuseau )

#endif // __MIL_Fuseau_h_
