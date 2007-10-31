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

struct ASN1T_Intelligence;

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
    explicit MIL_IntelligenceOrder( const ASN1T_Intelligence& asn );
    virtual ~MIL_IntelligenceOrder();
    //@}

    //! @name Operations
    //@{
    void Serialize( ASN1T_Intelligence& asn ) const;
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
    const ASN1T_EnumNatureLevel level_;
    const bool embarked_;
    const ASN1T_CoordUTM position_;
    const ASN1T_EnumDiplomacy diplomacy_;
    const unsigned int formation_;
    //@}
};

#endif // __MIL_IntelligenceOrder_h_
