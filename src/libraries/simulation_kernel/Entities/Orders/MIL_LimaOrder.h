// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_LimaOrder_h_
#define __MIL_LimaOrder_h_

#include "MT_Tools/MT_Vector2DTypes.h"

namespace Common
{
    class MsgLimaOrder;
}

class MT_Line;
class MIL_LimaFunction;
struct ASN1T_LimaOrder;

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
    explicit MIL_LimaOrder( const Common::MsgLimaOrder& asn );
    virtual ~MIL_LimaOrder();
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
          unsigned int     GetID       () const;
    const T_PointVector&   GetPoints   () const;
    const T_LimaFunctions& GetFunctions() const;
          unsigned int     GetSchedule () const;
    //@}

    //! @name Geometry $$$ BULLSHIT
    //@{
    bool Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const;
    bool Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const; //$$$ devrait gicler
    //@}

    //! @name Network
    //@{
    void Serialize( Common::MsgLimaOrder& asn ) const;
    //@}

private:
    unsigned int            nID_;
    T_PointVector   points_;
    T_LimaFunctions functions_;
    bool            bFlag_;
    bool            bScheduleFlag_;
    unsigned int            nSchedule_;

private:
    static unsigned int nNextID_;
};

typedef std::vector< MIL_LimaOrder  > T_LimaVector;
typedef T_LimaVector::iterator        IT_LimaVector;
typedef T_LimaVector::const_iterator  CIT_LimaVector;

#endif // __MIL_LimaOrder_h_
