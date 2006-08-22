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

namespace kernel
{
    class Agent_ABC;
}

class Simulation;

// =============================================================================
// Created: AGN 2003-12-22
// =============================================================================
class Trace : public Report_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             Trace( const kernel::Entity_ABC& agent, const Simulation& simulation, const TraceMessage& input );
    virtual ~Trace();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Trace( const Trace& );
    Trace& operator=( const Trace& );
    //@}
};


#endif // __Trace_h_
