//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Barricade.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:10 $
// $Revision: 6 $
// $Workfile: MIL_Barricade.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_Barricade.h"
#include "MIL_RealObjectType.h"

BOOST_CLASS_EXPORT_GUID( MIL_Barricade, "MIL_Barricade" )

//-----------------------------------------------------------------------------
// Name: MIL_Barricade constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Barricade::MIL_Barricade()
    : MIL_Obstacle( MIL_RealObjectType::barricade_ )
{
    nFullNbrDotationForMining_       = 10;
    nFullNbrDotationForConstruction_ = 1;
}

//-----------------------------------------------------------------------------
// Name: MIL_Barricade destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
MIL_Barricade::~MIL_Barricade()
{
	
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: MIL_Barricade::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_Barricade::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< MIL_RealObject_ABC >( *this );
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_Barricade::Initialize
// Created: JVT 02-10-22
//-----------------------------------------------------------------------------
bool MIL_Barricade::Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx )
{
    if( !MIL_RealObject_ABC::Initialize( army, diaParameters, nCurrentParamIdx ) )
        return false;

    rSizeCoef_                       = diaParameters[ nCurrentParamIdx ].ToFloat();
    nFullNbrDotationForConstruction_ = (uint)( rSizeCoef_ / 5. );  // Barbelés
    return true;
}


//-----------------------------------------------------------------------------
// Name: MIL_Barricade::Initialize
// Created: NLD 2003-07-21
//-----------------------------------------------------------------------------
void MIL_Barricade::Initialize( uint nID, MIL_InputArchive& archive )
{
    MIL_RealObject_ABC::Initialize( nID, archive );
    nFullNbrDotationForConstruction_    = (uint)( rSizeCoef_ / 5. );  // Barbelés
}

// -----------------------------------------------------------------------------
// Name: MIL_Barricade::Initialize
// Created: NLD 2003-08-04
// -----------------------------------------------------------------------------
ASN1T_EnumObjectErrorCode MIL_Barricade::Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject )
{
    ASN1T_EnumObjectErrorCode nErrorCode = MIL_RealObject_ABC::Initialize( nID, asnCreateObject );
    if( nErrorCode != EnumObjectErrorCode::no_error )
        return nErrorCode;

    nFullNbrDotationForConstruction_    = (uint)( rSizeCoef_ / 5. );  // Barbelés
    return EnumObjectErrorCode::no_error;
}

 