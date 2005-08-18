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

#ifndef __TacticalLine_Lima_h_
#define __TacticalLine_Lima_h_

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
/** @class  TacticalLine_Lima
    @brief  TacticalLine_Lima
    @par    Using example
    @code
    TacticalLine_Lima;
    @endcode
*/
// Created: SBO 2005-08-09
// =============================================================================
class TacticalLine_Lima : public TacticalLine_ABC
{

public:
    //! @name ID Managers
    //@{
    static IDManager idManager_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_Lima( const T_PositionVector& points );
             TacticalLine_Lima( const ASN1T_MsgLimaCreation& asnMsg );
    virtual ~TacticalLine_Lima();
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
    TacticalLine_Lima( const TacticalLine_Lima& );            //!< Copy constructor
    TacticalLine_Lima& operator=( const TacticalLine_Lima& ); //!< Assignment operator
    //@}

};

} // end namespace TEST

#include "TacticalLine_Lima.inl"

#endif // __TacticalLine_Lima_h_
