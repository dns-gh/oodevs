// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_RealObjectType_SiteDecontamination.cpp $
// $Author: Nld $
// $Modtime: 2/11/04 16:40 $
// $Revision: 1 $
// $Workfile: MIL_RealObjectType_SiteDecontamination.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "MIL_RealObjectType_SiteDecontamination.h"

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType_SiteDecontamination constructor
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
MIL_RealObjectType_SiteDecontamination::MIL_RealObjectType_SiteDecontamination( const std::string strName, E_ObjectType nType, ASN1T_EnumObjectType nAsnID, MIL_MOSIDManager& idManager, T_ObjectInstanciator objectInstanciator )
    : MIL_RealObjectType( strName, nType, nAsnID, idManager, objectInstanciator )
    , nNbrMaxEnablers_  ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType_SiteDecontamination destructor
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
MIL_RealObjectType_SiteDecontamination::~MIL_RealObjectType_SiteDecontamination()
{

}

// -----------------------------------------------------------------------------
// Name: MIL_RealObjectType_SiteDecontamination::Read
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void MIL_RealObjectType_SiteDecontamination::Read( MIL_InputArchive& archive )
{
    MIL_RealObjectType::Read( archive );
    archive.ReadField( "NombreAnimateursMax", nNbrMaxEnablers_, CheckValueGreater( 0 ) );
}
