// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLine_Lima_h_
#define __TacticalLine_Lima_h_

#include "Types.h"
#include "TacticalLine_ABC.h"
#include "Tools/Position.h"
#include "Tools/IDManager.h"
#include "Messages/ASN_Messages.h"

namespace TEST {

// =============================================================================
/** @class  TacticalLine_Lima
    @brief  TacticalLine_Lima
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
             TacticalLine_Lima( XmlInputArchive& points );
    virtual ~TacticalLine_Lima();
    //@}

    //! @name Accessors
    //@{
    virtual TacticalLine_ABC::E_LineType GetLineType() const;
            ASN1T_EnumLimaType           GetLimaType() const;
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

private:
    //! @name Member data
    //@{
    ASN1T_EnumLimaType eLimaType_;
    //@}

};

} // end namespace TEST

#include "TacticalLine_Lima.inl"

#endif // __TacticalLine_Lima_h_
