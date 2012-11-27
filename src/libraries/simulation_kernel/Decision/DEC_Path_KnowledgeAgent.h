// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Path_KnowledgeAgent_h_
#define __DEC_Path_KnowledgeAgent_h_

#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Line.h"
#include "MT_Tools/MT_Rect.h"

// =============================================================================
// Created: NLD 2004-04-06
// =============================================================================
class DEC_Path_KnowledgeAgent
{
public:
    //! @name Types
    //@{
    struct BoundingBox
    {
        BoundingBox( const MT_Vector2D& from, const MT_Vector2D& to )
        {
            if( from.rX_ < to.rX_ )
            {
                x1_ = from.rX_ - maxFireDistance_;
                x2_ = to.rX_ + maxFireDistance_;
            }
            else
            {
                x1_ = to.rX_ - maxFireDistance_;
                x2_ = from.rX_ + maxFireDistance_;
            }
            if( from.rY_ < to.rY_ )
            {
                y1_ = from.rY_ - maxFireDistance_;
                y2_ = to.rY_ + maxFireDistance_;
            }
            else
            {
                y1_ = to.rY_ - maxFireDistance_;
                y2_ = from.rY_ + maxFireDistance_;
            }
        }
        bool IsOutside( const MT_Vector2D& point ) const
        {
            return point.rX_ < x1_ || point.rX_ > x2_
                || point.rY_ < y1_ || point.rY_ > y2_;
        }
        double x1_;
        double y1_;
        double x2_;
        double y2_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeAgent( const MT_Vector2D& position, double enemyCostAtSecurityRange, double enemyCostOnContact, double maxRangeToFire );
    virtual ~DEC_Path_KnowledgeAgent();
    //@}

    //! @name Operations
    //@{
    double ComputeCost( const MT_Line& lineLink, const BoundingBox& box ) const;
    //@}

private:
    //! @name Member data
    //@{
    static const double maxFireDistance_;
    MT_Vector2D vEnemyPosition_;
    double rSquareSecurityDistance_;
    double rFactor_;
    //@}
};

#endif // __DEC_Path_KnowledgeAgent_h_
