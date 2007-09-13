// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimaOrder_h_
#define __LimaOrder_h_

#include "Localisation.h"

struct ASN1T_LimaOrder;

namespace dispatcher
{
// =============================================================================
/** @class  LimaOrder
    @brief  LimaOrder
*/
// Created: NLD 2007-04-20
// =============================================================================
class LimaOrder
{
public:
    //! @name Constructors/Destructor
    //@{
     LimaOrder( Model& model, const ASN1T_LimaOrder& asn );
    ~LimaOrder();
    //@}

    //! @name Operations
    //@{
           void Send     ( ASN1T_LimaOrder& asn ) const;
    static void AsnDelete( ASN1T_LimaOrder& asn );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LimaOrder( const LimaOrder& );            //!< Copy constructor
    LimaOrder& operator=( const LimaOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< ASN1T_EnumLimaType > T_Functions;
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    T_Functions  functions_;
    unsigned int nSchedule_;
    //@}
};

}

#endif // __LimaOrder_h_
