// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SelectedElement.h $
// $Author: Age $
// $Modtime: 5/04/05 18:17 $
// $Revision: 5 $
// $Workfile: MOS_SelectedElement.h $
//
// *****************************************************************************

#ifndef __MOS_SelectedElement_h_
#define __MOS_SelectedElement_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"

class MOS_Agent;
class MOS_Gtia;
class MOS_Team;
class MOS_Object_ABC;
class MOS_AgentKnowledge;
class MOS_ObjectKnowledge;
class MOS_RC;
class MOS_TacticalLine_ABC;


// =============================================================================
/** @class  MOS_SelectedElement
    @brief  Used to hold an object that was selected on the gui.
*/
// Created: APE 2004-05-05
// =============================================================================
class MOS_SelectedElement
{

public:
    //! @name Constructors/Destructor
    //@{
    MOS_SelectedElement();
    MOS_SelectedElement( MOS_Agent& agent );
    MOS_SelectedElement( MOS_Gtia& gtia );
    MOS_SelectedElement( MOS_Team& team );
    MOS_SelectedElement( MOS_Object_ABC& object );
    MOS_SelectedElement( MOS_AgentKnowledge& agentKnowledge );
    MOS_SelectedElement( MOS_ObjectKnowledge& ObjectKnowledge );
    MOS_SelectedElement( MOS_RC& rc );
    MOS_SelectedElement( MOS_TacticalLine_ABC& line, int nLinePoint = -1 );
    ~MOS_SelectedElement();
    //@}

    bool IsAMapElementSelected() const;

    bool operator==( const MOS_SelectedElement& ) const;

    void Dump();

private:
    void Init();

public:
    //! @name Public member data
    //@{
    MOS_Agent*               pAgent_;
    MOS_Gtia*                pGtia_;
    MOS_Team*                pTeam_;
    MOS_Object_ABC*      pObject_;
    MOS_AgentKnowledge*      pAgentKnowledge_;
    MOS_ObjectKnowledge* pObjectKnowledge_;
    MOS_RC*                  pRC_;
    MOS_TacticalLine_ABC*    pLine_;
    int                      nLinePoint_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_SelectedElement.inl"
#endif

#endif // __MOS_SelectedElement_h_
