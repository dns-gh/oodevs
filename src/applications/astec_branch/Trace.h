//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $ATracehive: /MVW_v10/Build/SDK/Light/sTrace/Trace.h $
// $Author: Ape $
// $Modtime: 4/08/04 12:04 $
// $Revision: 3 $
// $Workfile: Trace.h $
//
//*****************************************************************************

#ifndef __Trace_h_
#define __Trace_h_

#include "Report_ABC.h"
#include "DIN_Types.h"

class Agent_ABC;

// =============================================================================
// Created: AGN 2003-12-22
// =============================================================================
class Trace : public Report_ABC
{
    MT_COPYNOTALLOWED( Trace );

public:
    //! @name Constructor/Destructor
    //@{
     Trace( Agent_ABC& agent, const TraceMessage& input );
    ~Trace();
    //@}
};


#endif // __Trace_h_
