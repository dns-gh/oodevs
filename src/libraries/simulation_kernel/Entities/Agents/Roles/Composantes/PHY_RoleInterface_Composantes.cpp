// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.cpp $
// $Author: Jvt $
// $Modtime: 31/03/05 18:22 $
// $Revision: 2 $
// $Workfile: PHY_RoleInterface_Composantes.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleInterface_Composantes.h"

double PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_             = 0.;
double PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_                = 0.;
double PHY_RoleInterface_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = 0.;
double PHY_RoleInterface_Composantes::rMaxDangerosityDegradationByOpState_          = 0.;
double PHY_RoleInterface_Composantes::rOpStateDecisionalThreshold_                  = 0.;

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Composantes constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_RoleInterface_Composantes::PHY_RoleInterface_Composantes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleInterface_Composantes destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_RoleInterface_Composantes::~PHY_RoleInterface_Composantes()
{
    // NOTHING
}
