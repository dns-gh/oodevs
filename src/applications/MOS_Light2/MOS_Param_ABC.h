// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Param_ABC.h $
// $Author: Ape $
// $Modtime: 3/08/04 17:31 $
// $Revision: 5 $
// $Workfile: MOS_Param_ABC.h $
//
// *****************************************************************************

#ifndef __MOS_Param_ABC_h_
#define __MOS_Param_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"

class MOS_ActionContext;
class QPopupMenu;

class MOS_Param_ABC;
typedef std::vector< MOS_Param_ABC* >   T_ParamVector;
typedef T_ParamVector::iterator         IT_ParamVector;
typedef T_ParamVector::const_iterator   CIT_ParamVector;


// =============================================================================
/** @class  MOS_Param_ABC
    @brief  MOS_Param_ABC
    @par    Using example
    @code
    MOS_Param_ABC;
    @endcode
*/
// Created: APE 2004-03-18
// =============================================================================
class MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_Param_ABC );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Param_ABC();
    virtual ~MOS_Param_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Draw();
    virtual void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    virtual bool CheckValidity();
    virtual void WriteMsg ( std::stringstream& strMsg ) = 0;
    //@}
};

#   include "MOS_Param_ABC.inl"

#endif // __MOS_Param_ABC_h_
