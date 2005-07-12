//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Dialog_ABC.h $
// $Author: Nld $
// $Modtime: 28/11/02 14:12 $
// $Revision: 3 $
// $Workfile: MOS_Dialog_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_Dialog_ABC_h_
#define __MOS_Dialog_ABC_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include <qdialog.h>

class MOS_Agent;

class QComboBox;

//=============================================================================
/**
*   @class  MOS_Dialog_ABC
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created: NLD 2002-08-06
//=============================================================================
class MOS_Dialog_ABC : public QDialog
{
    MT_COPYNOTALLOWED( MOS_Dialog_ABC );

public:
    MOS_Dialog_ABC( QWidget* pParent = 0 );
    virtual ~MOS_Dialog_ABC();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize();
    virtual void Cleanup   ();
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void         InitPositionComboBox ( QComboBox& comboBox );
    void         InitItineraryComboBox( QComboBox& comboBox );
    std::string& ConvertPositionValue ( std::string& strValue );
    //@}   
};

#ifdef MOS_USE_INLINE
#   include "MOS_Dialog_ABC.inl"
#endif

#endif // __MOS_Dialog_ABC_h_
