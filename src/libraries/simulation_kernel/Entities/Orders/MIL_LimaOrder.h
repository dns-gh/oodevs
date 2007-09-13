// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_LimaOrder_h_
#define __MIL_LimaOrder_h_

#include "MIL.h"

class MIL_LimaFunction;

// =============================================================================
/** @class  MIL_LimaOrder
    @brief  MIL_LimaOrder
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_LimaOrder
{
public:
    //! @name Types
    //@{
    typedef std::set< const MIL_LimaFunction* > T_LimaFunctions;
    typedef T_LimaFunctions::const_iterator     CIT_LimaFunctions;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
     MIL_LimaOrder( const ASN1T_LimaOrder& asn );
    ~MIL_LimaOrder();
    //@}

    //! @name Operations
    //@{
    bool HasFunction      ( const MIL_LimaFunction& function ) const;
    void Flag             ( bool bFlag );
    bool IsFlagged        () const;
    void FlagSchedule     ( bool bFlag );
    bool IsScheduleFlagged() const;
    //@}

    //! @name Accessors
    //@{
          uint             GetID       () const;
    const T_PointVector&   GetPoints   () const;
    const T_LimaFunctions& GetFunctions() const;
          uint             GetSchedule () const;
    //@}

    //! @name Geometry $$$ BULLSHIT
    //@{
    bool Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const;
    bool Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const; //$$$ devrait gicler
    //@}

    //! @name Network
    //@{
    void Serialize( ASN1T_LimaOrder& asn ) const;
    //@}

private:
    uint            nID_;
    T_PointVector   points_;
    T_LimaFunctions functions_;
    bool            bFlag_;
    bool            bScheduleFlag_;
    uint            nSchedule_;

private:
    static uint nNextID_;
};

#include "MIL_LimaOrder.inl"

#endif // __MIL_LimaOrder_h_
