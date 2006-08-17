//*****************************************************************************
//
// $Created: AGN 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Genie/DEC_Gen_Object.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 17:00 $
// $Revision: 3 $
// $Workfile: DEC_Gen_Object.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Gen_Object.h"

#include "Network/NET_ASN_Tools.h"
#include "Decision/DEC_Tools.h"

int DEC_Gen_Object::nDIATypeIdx_         = 0;
int DEC_Gen_Object::nDIAPosIdx_          = 0;
int DEC_Gen_Object::nDIAUrgenceIdx_      = 0;
int DEC_Gen_Object::nDIAPreliminaireIdx_ = 0;
int DEC_Gen_Object::nDIAMosIDIdx_        = 0;
int DEC_Gen_Object::nDIAPrioriteIdx_     = 0;

//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object::InitializeDIA
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
//static
void DEC_Gen_Object::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_GenObjet" );
    nDIATypeIdx_         = DEC_Tools::InitializeDIAField( "typeObject_"    , diaType );
    nDIAPosIdx_          = DEC_Tools::InitializeDIAField( "posObject_"     , diaType );
    nDIAUrgenceIdx_      = DEC_Tools::InitializeDIAField( "urgence_"       , diaType );
    nDIAPreliminaireIdx_ = DEC_Tools::InitializeDIAField( "preliminaire_"  , diaType );
    nDIAMosIDIdx_        = DEC_Tools::InitializeDIAField( "nObstacleMosID_", diaType );
    nDIAPrioriteIdx_     = DEC_Tools::InitializeDIAField( "priorite_"      , diaType );
}


//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object constructor
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
DEC_Gen_Object::DEC_Gen_Object()
: DIA_Thing( DIA_Thing::ThingType(), *DIA_TypeManager::Instance().GetType( "T_GenObjet" ) )
{
    
}


//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object destructor
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
DEC_Gen_Object::~DEC_Gen_Object()
{
    
}

//-----------------------------------------------------------------------------
// Name: DEC_Gen_Object::Initialize
// Created: AGN 03-08-27
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode DEC_Gen_Object::Initialize( const ASN1T_MissionGenObject& asn )
{
    GetVariable( nDIATypeIdx_         ).SetValue( (int)asn.type_obstacle                                              );
    GetVariable( nDIAMosIDIdx_        ).SetValue( (void*)asn.oid_obstacle_planifie, &DEC_Tools::GetTypeID()          , 1 );
    GetVariable( nDIAPosIdx_          ).SetValue( (void*)&localisation_           , &DEC_Tools::GetTypeLocalisation(), 1 );
    GetVariable( nDIAPrioriteIdx_     ).SetValue( (int)asn.priorite                                                   );
    GetVariable( nDIAUrgenceIdx_      ).SetValue( (int)asn.urgence                                                    );
    GetVariable( nDIAPreliminaireIdx_ ).SetValue( (int)asn.preliminaire                                               );

    if( !NET_ASN_Tools::ReadLocation( asn.pos_obstacle, localisation_ ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    return EnumOrderErrorCode::no_error;
}

