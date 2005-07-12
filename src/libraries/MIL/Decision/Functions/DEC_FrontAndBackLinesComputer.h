// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FrontAndBackLinesComputer.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: DEC_FrontAndBackLinesComputer.h $
//
// *****************************************************************************

#ifndef __DEC_FrontAndBackLinesComputer_h_
#define __DEC_FrontAndBackLinesComputer_h_

#include "MIL.h"

class MIL_AgentPion;
class MIL_Automate;

// =============================================================================
// Created: NLD 2003-08-20
// =============================================================================
class DEC_FrontAndBackLinesComputer
{
    MT_COPYNOTALLOWED( DEC_FrontAndBackLinesComputer );

public:
     DEC_FrontAndBackLinesComputer( const MIL_Automate& caller, DIA_Call_ABC& call );
    ~DEC_FrontAndBackLinesComputer();

    //! @name Operations
    //@{
    MT_Float ComputeDistanceFromFrontLine( const MT_Vector2D& vPoint );
    MT_Float ComputeDistanceFromBackLine ( const MT_Vector2D& vPoint );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_AgentPion*> T_PionVector;
    typedef T_PionVector::const_iterator  CIT_PionVector;
    //@}

private:
    //! @name Operations
    //@{
    void Compute();
    //@}
                                              
private:
    const MIL_Automate& automate_;
          T_PionVector   pions_;
          uint           nLastTimeComputed_;

    MT_Droite backLineDroite_;
    MT_Droite frontLineDroite_;
};

#include "DEC_FrontAndBackLinesComputer.inl"

#endif // __DEC_FrontAndBackLinesComputer_h_
