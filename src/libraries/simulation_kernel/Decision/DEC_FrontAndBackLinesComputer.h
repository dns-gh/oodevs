// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __DEC_FrontAndBackLinesComputer_h_
#define __DEC_FrontAndBackLinesComputer_h_

#include "MT_Tools/MT_Droite.h"

class MIL_AgentPion;
class MIL_Automate;
class MT_Vector2D;

// =============================================================================
// Created: NLD 2003-08-20
// =============================================================================
class DEC_FrontAndBackLinesComputer : private boost::noncopyable
{
public:
             DEC_FrontAndBackLinesComputer( const MIL_Automate& caller, const std::vector< MIL_AgentPion*>& pions );
             DEC_FrontAndBackLinesComputer( const MIL_Automate& caller, const std::vector< MIL_Automate*>& automats );
    virtual ~DEC_FrontAndBackLinesComputer();

    //! @name Operations
    //@{
    double ComputeDistanceFromFrontLine( const MT_Vector2D& vPoint );
    double ComputeDistanceFromBackLine ( const MT_Vector2D& vPoint );
    void UsedByDIA();
    void ReleasedByDIA();
    //@}

private:
    //! @name Operations
    //@{
    void Compute();
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Automate& refAutomate_; // reference
    std::vector< MIL_AgentPion* > pions_; //$$$ Factoriser
    std::vector< MIL_Automate* > automates_;
    unsigned int nLastTimeComputed_;
    MT_Droite backLineDroite_;
    MT_Droite frontLineDroite_;
    //@}
};

#endif // __DEC_FrontAndBackLinesComputer_h_
