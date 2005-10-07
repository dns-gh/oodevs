//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_Trace.h $
// $Author: Ape $
// $Modtime: 4/08/04 12:04 $
// $Revision: 3 $
// $Workfile: MOS_Trace.h $
//
//*****************************************************************************

#ifndef __MOS_Trace_h_
#define __MOS_Trace_h_

#ifdef __GNUG__
#    pragma interface
#endif

#include "MOS_Report_ABC.h"

class MOS_Agent_ABC;


// =============================================================================
/** @class  MOS_Trace
    @brief  Debug message sent by one of the simulation's agent for display in
            the GUI.
*/
// Created: AGN 2003-12-22
// =============================================================================
class MOS_Trace : public MOS_Report_ABC
{
    MT_COPYNOTALLOWED( MOS_Trace );

public:
     MOS_Trace( MOS_Agent_ABC& agent );
    ~MOS_Trace();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( DIN::DIN_Input& input );
    //@}
};

#   include "MOS_Trace.inl"

#endif // __MOS_Trace_h_
