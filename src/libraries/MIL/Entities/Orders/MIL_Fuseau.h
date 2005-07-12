//*****************************************************************************
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

#include "MIL.h"
#include "TER/TER_Polygon.h"

class TER_Localisation;
class MIL_Limit;
class MIL_Lima;
class TER_DynamicData;

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

    typedef std::vector< const MIL_Limit* >       T_LimitConstPtrVector;
    typedef T_LimitConstPtrVector::const_iterator CIT_LimitConstPtrVector;
    //@}
    
public:
     MIL_Fuseau();
     MIL_Fuseau( const MT_Vector2D& vOrientationRefPos, const MIL_Limit& leftLimit, const MIL_Limit& rightLimit, const MIL_Lima* pBeginMissionLima, const MIL_Lima* pEndMissionLima );
    ~MIL_Fuseau();

    //! @name Init */
    //@{
    void Reset( const MT_Vector2D& vOrientationRefPos, const MIL_Limit& leftLimit, const MIL_Limit& rightLimit, const MIL_Lima* pBeginMissionLima, const MIL_Lima* pEndMissionLima );
    void Reset();
    //@}

    //! @name Geometry */
    //@{
    bool IsInside           ( const MT_Vector2D& vPos  ) const;
    bool IntersectWithBorder( const MT_Line& line ) const;
    bool IntersectWithBorder( const MT_Line& line, T_PointSet& res ) const;

    bool ComputeFurthestExtremityPoint( MT_Vector2D& vResult ) const;
    bool ComputeClosestExtremityPoint ( MT_Vector2D& vResult ) const;
    bool ComputePointBeforeLima       ( const MIL_Lima& lima, MT_Float rDistBefore, MT_Vector2D&   vResult ) const;
    bool ComputePointsBeforeLima      ( const MIL_Lima& lima, MT_Float rDistBefore, uint nNbPoints, T_PointVector& results ) const;
    void ComputeNearestEntryPoint     ( const MT_Vector2D& vStartPos, MT_Vector2D& vResult ) const;
    void ComputeEntryPoint            ( const MT_Vector2D& vStartPos, MT_Vector2D& vResult ) const;

    MT_Float GetCost( const MT_Vector2D& from, const MT_Vector2D& to, MT_Float rMaxDistance ) const;
    MT_Float Distance( const MT_Vector2D& p, bool bLimitsOnly = false ) const;
    //@}

    //! @name Accessors */
    //@{
    const MIL_Limit* GetLeftLimit      () const;
    const MIL_Limit* GetRightLimit     () const;
    bool             IsNull            () const;
    const MT_Line&   GetGlobalDirection() const;
    //@}
   
    //! @name Tools */
    //@{
    bool SplitIntoSubFuseaux( uint nNbrSubFuseau, T_FuseauPtrList& container ) const;
    TER_DynamicData* CreateDynamicData() const;
    //@}

    //! @name Operators */
    //@{
    MIL_Fuseau& operator=( const MIL_Fuseau& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< T_PointVector >  T_PointVectorVector;
    typedef T_PointVectorVector::iterator IT_PointVectorVector;
    //@}

private:
     MIL_Fuseau( const MIL_Fuseau& rhs ); // ????

    //! @name Init
    //@{
    void InitializePolygon        ();
    void TruncateAndReorientLimits();
    void SplitLimit               ( const T_PointVector& limit, T_PointVectorVector& parts );
    bool IsPointInsidePolygon     ( T_PointVector& leftPoints, T_PointVector& rightPoints, const MT_Vector2D& vPoint );
    //@}

    //! @name Tools
    //@{
    bool Split( uint nNbrSubFuseau, T_LimitConstPtrVector& limitVector ) const;
    //@}

private:
    const MIL_Limit* pLeftLimit_;
    const MIL_Limit* pRightLimit_;
    const MIL_Lima*  pBeginMissionLima_;
    const MIL_Lima*  pEndMissionLima_;

    T_PointVector leftPointVector_;
    T_PointVector rightPointVector_;

    MT_Float    rLeftLimitLength_;
    MT_Float    rRightLimitLength_;

    // Fuseau global direction
    MT_Vector2D vOrientationRefPos_;
    MT_Line     globalDirectionLine_;
    MT_Vector2D vStartGlobalDirection_;
    MT_Vector2D vEndGlobalDirection_;

private:
    static uint nNbrMeterPerSample_;
};

#include "MIL_Fuseau.inl"

#endif // __MIL_Fuseau_h_
