// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationLogisticType.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 8 $
// $Workfile: PHY_DotationLogisticType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_DotationLogisticType.h"
#include "MT_Tools/MT_Logger.h"

PHY_DotationLogisticType::T_DotationLogisticTypeMap PHY_DotationLogisticType::dotationLogisticTypes_;

//-----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationLogisticType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing logistic dotation types" );
    xis >> xml::start( "logistic-supply-classes" )
            >> xml::list( "logistic-supply-class", &PHY_DotationLogisticType::ReadDotationLogisticType )
        >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Terminate
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationLogisticType::Terminate()
{
    dotationLogisticTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::ReadDotationLogisticType
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationLogisticType::ReadDotationLogisticType( xml::xistream& xis )
{
    unsigned int id;
    std::string type;

    xis >> xml::attribute( "id", id )
        >> xml::attribute( "type", type );

    const PHY_DotationLogisticType*& pData = dotationLogisticTypes_[ type ];
    if( pData )
        xis.error( "Logistic supply class '" + type + "' already registered" );
    pData = new PHY_DotationLogisticType( id, type );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType constructor
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationLogisticType::PHY_DotationLogisticType( unsigned int id, const std::string& type )
    : id_  ( id )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType destructor
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
PHY_DotationLogisticType::~PHY_DotationLogisticType()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Find
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType* PHY_DotationLogisticType::Find( const std::string& type )
{
    CIT_DotationLogisticTypeMap it = dotationLogisticTypes_.find( type );
    if( it == dotationLogisticTypes_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::Find
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType* PHY_DotationLogisticType::Find( unsigned int nID )
{
    for( CIT_DotationLogisticTypeMap it = dotationLogisticTypes_.begin(); it != dotationLogisticTypes_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetDotationLogisticTypes
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const PHY_DotationLogisticType::T_DotationLogisticTypeMap& PHY_DotationLogisticType::GetDotationLogisticTypes()
{
    return dotationLogisticTypes_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetID
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
unsigned int PHY_DotationLogisticType::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationLogisticType::GetType
// Created: NLD 2006-01-03
// -----------------------------------------------------------------------------
const std::string& PHY_DotationLogisticType::GetType() const
{
    return type_;
}
