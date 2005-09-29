// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendComposante.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:21 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendComposante.h $
//
// *****************************************************************************

#ifndef __PHY_ActionLendComposante_h_
#define __PHY_ActionLendComposante_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RolePion_Composantes;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionLendComposante
// Created: JVT 2005-05-12
// =============================================================================
class PHY_ActionLendComposante : public PHY_Action_ABC
{
public:
    typedef MIL_AgentPion ActorType;        

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionLendComposante( MIL_AgentPion&, DIA_Call_ABC& );
    virtual ~PHY_ActionLendComposante();
    //@}

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PHY_ActionLendComposante( const PHY_ActionLendComposante& );              //!< Copy constructor
    PHY_ActionLendComposante& operator = ( const PHY_ActionLendComposante& ); //!< Assignement operator
    //@}

private:
    PHY_RolePion_Composantes& role_;
    
    const uint                nNbrComposantes_;
          uint                nTimer_;
    PHY_RolePion_Composantes* pTarget_;
    DIA_Variable_ABC&         diaReturnVariable_;
};

#include "PHY_ActionLendComposante.inl"

#endif // __PHY_ActionLendComposante_h_
