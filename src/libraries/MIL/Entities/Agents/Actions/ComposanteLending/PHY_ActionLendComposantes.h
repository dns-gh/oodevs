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

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RolePion_Composantes;
class PHY_ComposantePion;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionLendComposantes
// Created: JVT 2005-05-12
// =============================================================================
class PHY_ActionLendComposantes : public PHY_Action_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;        
    //@}

protected:
    //! @name Types
    //@{
    typedef bool (PHY_ComposantePion::*T_ComposantePredicate)() const;
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionLendComposantes( MIL_AgentPion&, DIA_Call_ABC& , T_ComposantePredicate predicate );
    virtual ~PHY_ActionLendComposantes();
    //@}

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PHY_ActionLendComposantes( const PHY_ActionLendComposantes& );              //!< Copy constructor
    PHY_ActionLendComposantes& operator = ( const PHY_ActionLendComposantes& ); //!< Assignement operator
    //@}

private:
    PHY_RolePion_Composantes& role_;
    
    const uint                nNbrToLend_;
          uint                nTimer_;
    PHY_RolePion_Composantes* pTarget_;
    DIA_Variable_ABC&         diaReturnVariable_;
    T_ComposantePredicate     predicate_;
};

#endif // __PHY_ActionLendComposantes_h_
