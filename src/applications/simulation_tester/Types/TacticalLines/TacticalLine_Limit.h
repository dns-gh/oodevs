// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_Limit_h_
#define __TacticalLine_Limit_h_

#include "Types.h"
#include "TacticalLine_ABC.h"
#include "Tools/Position.h"
#include "Tools/IDManager.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

// =============================================================================
/** @class  TacticalLine_Limit
    @brief  TacticalLine_Limit
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
             TacticalLine_Limit( const T_PositionVector& points );
             TacticalLine_Limit( const MsgLimitCreation& asnMsg );
             TacticalLine_Limit( XmlInputArchive& points );
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

} // end namespace TEST

#include "TacticalLine_Limit.inl"

#endif // __TacticalLine_Limit_h_