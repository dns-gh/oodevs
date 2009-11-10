// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_HumanProtection.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 14:04 $
// $Revision: 7 $
// $Workfile: PHY_HumanProtection.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_HumanProtection.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Entities/Objects/MIL_MedicalTreatmentType.h"
#include "tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"

#include "Entities/Objects/MIL_MedicalTreatmentType.h"



PHY_HumanProtection::T_HumanProtectionMap   PHY_HumanProtection::protections_;
uint                                        PHY_HumanProtection::nNextID_;
MT_Random                                   PHY_HumanProtection::random_;

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_HumanProtection::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing human protections" );

    xis >> xml::start( "protections" )
            >> xml::list( "protection" , &ReadProtection )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection::ReadProtection
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_HumanProtection::ReadProtection( xml::xistream& xis )
{
    std::string strProtection;
    xis >> xml::attribute( "name", strProtection );
    
    const PHY_HumanProtection*& pProtection = protections_[ strProtection ];
    if( pProtection )
        xis.error( "Protection " + strProtection + " already defined" );

     pProtection = new PHY_HumanProtection( strProtection, xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection::ReadProtection
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_HumanProtection::ReadHumanProtection( xml::xistream& xis )
{
    std::string strInjury;
    xis >> xml::attribute( "name", strInjury );
    
    if( MIL_MedicalTreatmentType::Find( strInjury ) == 0 )
        xis.error( "The injury, which name is " + strInjury + " , isn't the name of a medical treatment" );
    protectionDescription_.injuryName_ = strInjury;

    xis >> xml::start( "types" )
            >> xml::list( "type", *this, &PHY_HumanProtection::ReadType )
        >> xml::end()
        >> xml:: start( "effects" )
            >> xml::list( "effect", *this, &PHY_HumanProtection::ReadEffect )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection::ReadProtection
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_HumanProtection::ReadType( xml::xistream& xis )
{
    std::string strName;
    xis >> xml::attribute( "name" , strName );
    protectionDescription_.injuryTypes_.push_back( strName );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection::ReadProtection
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_HumanProtection::ReadEffect( xml::xistream& xis )
{
    float threshold, protection_decrease_rate;
    xis >> xml::attribute( "threshold" , threshold );
    xis >> xml::attribute( "protection-decrease-rate" , protection_decrease_rate );
    protectionDescription_.injuryEffects_.push_back( std::make_pair( threshold , protection_decrease_rate ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection::Terminate
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void PHY_HumanProtection::Terminate()
{
    for( IT_HumanProtectionMap it = protections_.begin(); it != protections_.end(); ++it )
        delete it->second;
    protections_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_HumanProtection::PHY_HumanProtection( const std::string& strName, xml::xistream& xis )
    : strName_              ( strName )
    , nID_                  ( nNextID_ ++ )
    , protectionDescription_()
{
    std::string type;
    xis >> xml::attribute( "type", type );
    if( type != "humain" )
        xis.error( "The protection isn't for a human. Check out the protection type in the xml file");

    xis >> xml::start( "injuries" )
            >> xml::list( "injury", *this, &PHY_HumanProtection::ReadHumanProtection )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_HumanProtection::PHY_HumanProtection( const PHY_HumanProtection& humanProtection )
    : strName_              ( humanProtection.strName_ )
    , nID_                  ( humanProtection.nID_ )
    , protectionDescription_( humanProtection.protectionDescription_ )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection destructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
PHY_HumanProtection::~PHY_HumanProtection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection ComputeProtectionValue
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
float PHY_HumanProtection::ComputeProtectionValue( int injuryID , int threshold , const std::string type ) const
{   
    MIL_MedicalTreatmentType::Find( injuryID )->GetName();
    float protectionValue = 0;
    CIT_HumanProtectionMap it = protections_.find( strName_ );
    //If the protection which name is strName_ is found
    //If this protection protects against the injury which ID is injuryID
    //If this protection protects against the special type which causes the injury (example: the NBC agent for a NBC cloud, the class for a fire)
    if( it != protections_.end() && MIL_MedicalTreatmentType::Find( injuryID )->GetName() == it->second->protectionDescription_.injuryName_ && IsProtectionAgainstThisType( type , it->second->protectionDescription_.injuryTypes_ ) )
    {
        protectionValue = 1;
        CIT_InjuryEffects iter = it->second->protectionDescription_.injuryEffects_.begin();
        CIT_InjuryEffects iterJustBehind = it->second->protectionDescription_.injuryEffects_.begin();
        while( (*iter).first < threshold )
        {
            if( iter != it->second->protectionDescription_.injuryEffects_.begin() )
            {
                protectionValue -= (*iter).first * (*iter).second;
                ++iter;
            }
            else
            {
                protectionValue -= ( (*iter).first - (*iterJustBehind).first ) * (*iter).second;
                ++iter;
                ++iterJustBehind;
            }
    
        }
        protectionValue -= ( threshold - (*iterJustBehind).first ) * (*iterJustBehind).second;
    }
    return ( protectionValue > 0 ) ? protectionValue : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection Find
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_HumanProtection* PHY_HumanProtection::Find( const std::string& strName )
{
    CIT_HumanProtectionMap it = protections_.find( strName );
    if( it == protections_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanProtection GetProtections
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_HumanProtection::T_HumanProtectionMap& PHY_HumanProtection::GetProtections()
{
    return protections_;
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::InstanciateSensor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_HumanProtection& PHY_HumanProtection::InstanciateHumanProtection() const
{
    return *new PHY_HumanProtection( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::GetName
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
const std::string PHY_HumanProtection::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_SensorType::IsProtectionAgainstThisType
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
bool PHY_HumanProtection::IsProtectionAgainstThisType( const std::string type , T_InjuryTypes injuryTypesList ) const
{
    for( CIT_InjuryTypes it = injuryTypesList.begin() ; it != injuryTypesList.end() ; ++it)
    {
        if( (*it) == type )
            return true;
    }
    return false;
}