// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_IntelligenceOrder_h_
#define __MIL_IntelligenceOrder_h_

#include "protocol/Protocol.h"

class MIL_Fuseau;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
/** @class  MIL_IntelligenceOrder
    @brief  MIL_IntelligenceOrder
*/
// Created: SBO 2007-10-30
// =============================================================================
class MIL_IntelligenceOrder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_IntelligenceOrder( const sword::Intelligence& asn );
    virtual ~MIL_IntelligenceOrder();
    //@}

    //! @name Operations
    //@{
    bool IsInside( const MIL_Fuseau& fuseau ) const;
    bool IsInside( const TER_Localisation& location ) const;
    double SquareDistance( const MT_Vector2D& point ) const;
    MT_Vector2D ComputeDirection( const MT_Vector2D& origin ) const;
    bool IsOnFlank( const MIL_Fuseau& fuseau, bool left, bool right ) const;
    bool IsEmbarked() const;
    bool IsEnemy() const;
    bool IsFriend() const;
    sword::EnumNatureLevel GetLevel() const;
    void Serialize( sword::Intelligence& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_IntelligenceOrder& operator=( const MIL_IntelligenceOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const std::string nature_;
    const sword::EnumNatureLevel level_;
    const bool embarked_;
    const sword::CoordLatLong position_;
    const sword::EnumDiplomacy diplomacy_;
    const unsigned int formation_;
    //@}
};

#endif // __MIL_IntelligenceOrder_h_
