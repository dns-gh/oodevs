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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamDirection.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 3 $
// $Workfile: ParamDirection.h $
//
// *****************************************************************************

#ifndef __ParamDirection_h_
#define __ParamDirection_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class QDial;


// =============================================================================
/** @class  ParamDirection
    @brief  ParamDirection
    @par    Using example
    @code
    ParamDirection;
    @endcode
*/
// Created: APE 2004-08-03
// =============================================================================
class ParamDirection : public QHBox, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamDirection );

public:
    //! @name Constructors/Destructor
    //@{
     ParamDirection( ASN1T_Direction& asnDirection, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamDirection();
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

#endif // __ParamDirection_h_
