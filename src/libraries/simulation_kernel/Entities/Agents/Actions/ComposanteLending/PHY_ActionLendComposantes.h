// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendComposantes.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:21 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendComposantes.h $
//
// *****************************************************************************

#ifndef __PHY_ActionLendComposantes_h_
#define __PHY_ActionLendComposantes_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_ComposantePion;
class MIL_AgentPion;
class DEC_Decision_ABC;

// =============================================================================
// @class  PHY_ActionLendComposantes
// Created: JVT 2005-05-12
// =============================================================================
class PHY_ActionLendComposantes : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

protected:
    //! @name Types
    //@{
    typedef std::function< bool( const PHY_ComposantePion& ) > T_ComposantePredicate;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionLendComposantes( MIL_AgentPion&, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pTarget, unsigned int nbrToLend , const T_ComposantePredicate& predicate );
    virtual ~PHY_ActionLendComposantes();
    //@}

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_ActionLendComposantes( const PHY_ActionLendComposantes& );              //!< Copy constructor
    PHY_ActionLendComposantes& operator = ( const PHY_ActionLendComposantes& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MIL_AgentPion&            pion_;
    MIL_AgentPion&            target_;
    T_ComposantePredicate     predicate_;
    const unsigned int        nNbrToLend_;
    unsigned int              nTimer_;
    bool                      bLoanDone_;
    //@}
};

#endif // __PHY_ActionLendComposantes_h_
