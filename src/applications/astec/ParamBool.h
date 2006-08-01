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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamBool.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 2 $
// $Workfile: ParamBool.h $
//
// *****************************************************************************

#ifndef __ParamBool_h_
#define __ParamBool_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"


// =============================================================================
// Created: APE 2004-04-21
// =============================================================================
class ParamBool : public QCheckBox, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamBool );

public:
    //! @name Constructors/Destructor
    //@{
    ParamBool( ASN1BOOL& asnBool, bool bDefaultValue, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamBool();
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

#endif // __ParamBool_h_
