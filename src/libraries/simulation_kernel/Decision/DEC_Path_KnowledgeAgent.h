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

// =============================================================================
// Created: NLD 2004-04-06
// =============================================================================
class DEC_Path_KnowledgeAgent
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Path_KnowledgeAgent( const MT_Vector2D& position, double enemyCostAtSecurityRange, double enemyCostOnContact, double maxRangeToFire );
    virtual ~DEC_Path_KnowledgeAgent();
    //@}

    //! @name Operations
    //@{
    double ComputeCost( const MT_Line& lineLink ) const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D vEnemyPosition_;
    double rSquareSecurityDistance_;
    double rFactor_;
    double rOffset_;
    //@}
};

#endif // __DEC_Path_KnowledgeAgent_h_
