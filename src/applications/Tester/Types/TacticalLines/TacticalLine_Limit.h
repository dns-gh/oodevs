// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __TacticalLine_Limit_h_
#define __TacticalLine_Limit_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "TacticalLine_ABC.h"
#include "Tools/Position.h"
#include "Tools/IDManager.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

// =============================================================================
/** @class  TacticalLine_Limit
    @brief  TacticalLine_Limit
    @par    Using example
    @code
    TacticalLine_Limit;
    @endcode
*/
// Created: SBO 2005-08-09
// =============================================================================
class TacticalLine_Limit : public TacticalLine_ABC
{

public:
    //! @name ID Managers
    //@{
    static IDManager idManager_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_Limit( T_PositionVector& points );
             TacticalLine_Limit( const ASN1T_MsgLimitCreation& asnMsg );
    virtual ~TacticalLine_Limit();
    //@}

    //! @name Accessors
    //@{
    virtual TacticalLine_ABC::E_LineType GetLineType() const;
    //@}

    //! @name Operations
    //@{
    virtual void UpdateToSim();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLine_Limit( const TacticalLine_Limit& );            //!< Copy constructor
    TacticalLine_Limit& operator=( const TacticalLine_Limit& ); //!< Assignment operator
    //@}
};

} // end namespace TacticalLine

#include "TacticalLine_Limit.inl"

#endif // __TacticalLine_Limit_h_
