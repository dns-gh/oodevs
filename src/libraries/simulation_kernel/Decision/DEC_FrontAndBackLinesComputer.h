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

#include "MIL.h"

class MIL_AgentPion;
class MIL_Automate;

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
    MT_Float ComputeDistanceFromFrontLine( const MT_Vector2D& vPoint );
    MT_Float ComputeDistanceFromBackLine ( const MT_Vector2D& vPoint );
    void UsedByDIA();
    void ReleasedByDIA();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_AgentPion* >   T_PionVector;
    typedef T_PionVector::const_iterator  CIT_PionVector;

    typedef std::vector< MIL_Automate* >       T_AutomateVector;
    typedef T_AutomateVector::const_iterator CIT_AutomateVector;
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
    T_PionVector pions_; //$$$ Factoriser
    T_AutomateVector  automates_;
    unsigned int nLastTimeComputed_;
    MT_Droite backLineDroite_;
    MT_Droite frontLineDroite_;
    //@}
};

#endif // __DEC_FrontAndBackLinesComputer_h_
