// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Specialisations/Asa/MIL_AutomateTypeASA.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 11:44 $
// $Revision: 4 $
// $Workfile: MIL_AutomateTypeASA.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateTypeASA.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Brain.h"

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA constructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeASA::MIL_AutomateTypeASA( const std::string& strName, xml::xistream& xis )
    : MIL_AutomateType( strName, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA destructor
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
MIL_AutomateTypeASA::~MIL_AutomateTypeASA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA::Create
// Created: NLD 2004-09-14
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateTypeASA::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateTypeASA( strName, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA::GetArchetypeName
// Created: FAI 2014-10-07
// -----------------------------------------------------------------------------
std::string MIL_AutomateTypeASA::GetArchetypeName() const
{
    return "MIL_AutomateTypeASA";
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateTypeASA::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AutomateTypeASA::RegisterFunctions( sword::Brain& brain, MIL_Automate& automat ) const
{
    brain.RegisterFunction( "DEC_Geometrie_PosDeploiementASAOmni",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( int, const MT_Vector2D*, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePosDeploiementASAOmni, boost::cref( automat ), _1, _2, _3 ) ) );
}
