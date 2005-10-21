// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamBool.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 2 $
// $Workfile: MOS_ParamBool.h $
//
// *****************************************************************************

#ifndef __MOS_ParamBool_h_
#define __MOS_ParamBool_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"


// =============================================================================
// Created: APE 2004-04-21
// =============================================================================
class MOS_ParamBool : public QCheckBox, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamBool );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamBool( ASN1BOOL& asnBool, bool bDefaultValue, const std::string& strLabel, QWidget* pParent );
    ~MOS_ParamBool();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    ASN1BOOL& asnBool_;
    //@}
};

#endif // __MOS_ParamBool_h_
