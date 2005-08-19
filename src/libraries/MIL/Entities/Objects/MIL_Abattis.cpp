//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Abattis.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 12:44 $
// $Revision: 4 $
// $Workfile: MIL_Abattis.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_Abattis.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_Abattis, "MIL_Abattis" )

//-----------------------------------------------------------------------------
// Name: MIL_Abattis constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Abattis::MIL_Abattis()
    : MIL_Obstacle( MIL_RealObjectType::abattis_ )
{    
    nFullNbrDotationForConstruction_ = 50;   // Explosifs
    nFullNbrDotationForMining_       = 10;   // Mines
}

//-----------------------------------------------------------------------------
// Name: MIL_Abattis destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Abattis::~MIL_Abattis()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Abattis::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Abattis::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}


//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Abattis::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_Abattis::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
        return false;

    rSizeCoef_  = diaParameters[ nCurrentParamIdx ].ToFloat();  // Lengths
    return true;
}

