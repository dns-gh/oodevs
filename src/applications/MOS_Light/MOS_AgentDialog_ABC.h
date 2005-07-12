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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentDialog_ABC.h $
// $Author: Nld $
// $Modtime: 28/11/02 14:05 $
// $Revision: 2 $
// $Workfile: MOS_AgentDialog_ABC.h $
//
//*****************************************************************************

#ifndef __MOS_AgentDialog_ABC_h_
#define __MOS_AgentDialog_ABC_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Types.h"

#include "MOS_Dialog_ABC.h"

class MOS_Agent;

class QComboBox;

//=============================================================================
/**
*   @class  MOS_AgentDialog_ABC
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created: NLD 2002-08-06
//=============================================================================
class MOS_AgentDialog_ABC : public MOS_Dialog_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentDialog_ABC );

public:
    MOS_AgentDialog_ABC( QWidget* pParent = 0 );
    virtual ~MOS_AgentDialog_ABC();

    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize( MOS_Agent& agent );
    virtual void Cleanup   ();
    //@}

protected:
    MOS_Agent* pAgent_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_AgentDialog_ABC.inl"
#endif

#endif // __MOS_AgentDialog_ABC_h_
