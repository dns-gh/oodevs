// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamDirection.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 3 $
// $Workfile: MOS_ParamDirection.h $
//
// *****************************************************************************

#ifndef __MOS_ParamDirection_h_
#define __MOS_ParamDirection_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class QDial;


// =============================================================================
/** @class  MOS_ParamDirection
    @brief  MOS_ParamDirection
    @par    Using example
    @code
    MOS_ParamDirection;
    @endcode
*/
// Created: APE 2004-08-03
// =============================================================================
class MOS_ParamDirection : public QHBox, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamDirection );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ParamDirection( ASN1T_Direction& asnDirection, const std::string& strLabel, QWidget* pParent );
    ~MOS_ParamDirection();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_Direction& asnDirection_;

    QDial* pDial_;
    //@}
};

#   include "MOS_ParamDirection.inl"

#endif // __MOS_ParamDirection_h_
