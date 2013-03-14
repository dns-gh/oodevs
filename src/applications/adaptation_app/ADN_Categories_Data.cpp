//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Categories_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 16:12 $
// $Revision: 12 $
// $Workfile: ADN_Categories_Data.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Categories_Data.h"
#include "ADN_ConsistencyChecker.h"
#include "ADN_Workspace.h"
#include "ADN_Project_Data.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>

tools::IdManager ADN_Categories_Data::idManager_;

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_Data::ADN_Categories_Data()
    : ADN_Data_ABC( eCategories )
{
    vArmors_.AddUniquenessChecker( eError, duplicateName_ );
    vSizes_.AddUniquenessChecker( eError, duplicateName_ );
    vDotationNatures_.AddUniquenessChecker( eError, duplicateName_ );
    vLogisticSupplyClasses_.AddUniquenessChecker( eError, duplicateName_ );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_Data::~ADN_Categories_Data()
{
    Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::FilesNeeded
// Created: JDY 03-09-08
//-----------------------------------------------------------------------------
void ADN_Categories_Data::FilesNeeded(tools::Path::T_Paths& files) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szSizes_ );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szArmors_ );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szDotationNatures_ );
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLogisticSupplyClasses_ );
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Reset
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Reset()
{
    idManager_.Reset();
    vArmors_.Reset();
    vSizes_.Reset();
    vDotationNatures_.Reset();
    vLogisticSupplyClasses_.Reset();
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Load
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Load( const tools::Loader_ABC& fileLoader )
{
    const ADN_Project_Data::DataInfos& dataInfos = ADN_Workspace::GetWorkspace().GetProject().GetDataInfos();
    const tools::Path workingDirectory = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
    const tools::Path szArmorsFile = workingDirectory / dataInfos.szArmors_;
    fileLoader.LoadFile( szArmorsFile, boost::bind( &ADN_Categories_Data::ReadArmors, this, _1 ) );

    const tools::Path szSizesFile =  workingDirectory / dataInfos.szSizes_;
    fileLoader.LoadFile( szSizesFile, boost::bind( &ADN_Categories_Data::ReadSizes, this, _1 ) );

    const tools::Path szDotationNaturesFile =  workingDirectory / dataInfos.szDotationNatures_;
    fileLoader.LoadFile( szDotationNaturesFile, boost::bind( &ADN_Categories_Data::ReadDotationNatures, this, _1 ) );

    const tools::Path szLogisticSupplyClassesFile =  workingDirectory / dataInfos.szLogisticSupplyClasses_;
    fileLoader.LoadFile( szLogisticSupplyClassesFile, boost::bind( &ADN_Categories_Data::ReadLogisticSupplyClasses, this, _1 ) );

    vSizes_.CheckValidity();
    vArmors_.CheckValidity();
    vDotationNatures_.CheckValidity();
    vLogisticSupplyClasses_.CheckValidity();
}

//-----------------------------------------------------------------------------
// Name: ADN_Categories_Data::Save
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_Data::Save()
{
    if( vArmors_.GetErrorStatus() == eError ||
        vSizes_.GetErrorStatus() == eError ||
        vDotationNatures_.GetErrorStatus() == eError ||
        vLogisticSupplyClasses_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( GetInvalidDataErrorMsg() );

    const ADN_Project_Data::DataInfos& dataInfos = ADN_Workspace::GetWorkspace().GetProject().GetDataInfos();
    const tools::Path saveDirectory = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory();

    {
        tools::Path szArmorFile = saveDirectory / dataInfos.szArmors_;
        szArmorFile.Parent().CreateDirectories();
        tools::Xofstream output( szArmorFile );
        WriteArmors( output );
    }
    {
        tools::Path szSizesFile = saveDirectory / dataInfos.szSizes_;
        szSizesFile.Parent().CreateDirectories();
        tools::Xofstream output( szSizesFile );
        WriteSizes( output );
    }
    {
        tools::Path szNaturesFile = saveDirectory / dataInfos.szDotationNatures_;
        szNaturesFile.Parent().CreateDirectories();
        tools::Xofstream output( szNaturesFile );
        WriteDotationNatures( output );
    }
    {
        tools::Path szLogisticSupplyClassesFile = saveDirectory / dataInfos.szLogisticSupplyClasses_;
        szLogisticSupplyClassesFile.Parent().CreateDirectories();
        tools::Xofstream output( szLogisticSupplyClassesFile );
        WriteLogisticSupplyClasses( output );
    }
}

class ADN_String_Cmp : public std::unary_function< ADN_Type_String* , bool >
{
public:
    ADN_String_Cmp(const std::string& val) : val_(val) {}
    virtual ~ADN_String_Cmp() {}

    template< typename T >
    bool operator()( T* infos ) const
    {
        return infos && infos->strName_.GetData() == val_;
    }

private:
    std::string val_;
};

namespace
{
    template< typename Vector, typename Extractor >
    bool HasDuplicates( const Vector& vect, const Extractor& extractor )
    {
        std::vector< std::string > tmp;
        tmp.reserve( vect.size() );
        for( Vector::const_iterator it = vect.begin(); it != vect.end(); ++it )
            tmp.push_back( extractor( **it ) );

        std::sort( tmp.begin(), tmp.end() );
        std::vector< std::string >::iterator itNewEnd = std::unique( tmp.begin(), tmp.end() );
        size_t total = std::distance( tmp.begin(), itNewEnd );
        return total != tmp.size();
    }

    struct StringExtractor
    {
        template< typename T >
        std::string operator()( T& infos ) const
        {
            return infos.strName_.GetData();
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadVolume
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadVolume( xml::xistream& input )
{
    std::string strName = input.attribute< std::string >( "name" );
    T_SizeInfos_Vector::iterator foundSize = std::find_if( vSizes_.begin(), vSizes_.end(), ADN_String_Cmp( strName ) );
    if( foundSize != vSizes_.end() )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated volume type name '%1'" ).arg( strName.c_str() ).toStdString() );

    SizeInfos* pNewArmor = new SizeInfos();
    pNewArmor->strName_ = strName;
    vSizes_.AddItem( pNewArmor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadSizes
// Created: AGN 2004-07-01
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadSizes( xml::xistream& input )
{
    input >> xml::start( "volumes" )
            >> xml::list( "volume", *this, &ADN_Categories_Data::ReadVolume )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadProtection
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadProtection( xml::xistream& input )
{
    std::auto_ptr< helpers::ArmorInfos > spNewArmor( new helpers::ArmorInfos() );
    spNewArmor->ReadArchive( input );
    vArmors_.AddItem( spNewArmor.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadArmors
// Created: APE 2004-11-09
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadArmors( xml::xistream& input )
{
    input >> xml::start( "protections" )
            >> xml::list( "protection", *this, &ADN_Categories_Data::ReadProtection )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadNature
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadNature( xml::xistream& input )
{
    std::string strName;
    int id ( 0 );
    input >> xml::attribute( "type", strName );
    input >> xml::optional >> xml::attribute( "id", id );
    helpers::T_ResourceNatureInfos_Vector::iterator found = std::find_if( vDotationNatures_.begin(), vDotationNatures_.end(), ADN_String_Cmp( strName ) );
    if( found != vDotationNatures_.end() )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated resource nature type name '%1'" ).arg( strName.c_str() ).toStdString() );
    if( !id )
        id = idManager_.GetNextId();
    helpers::ResourceNatureInfos* pNew = new helpers::ResourceNatureInfos( strName, id );
    vDotationNatures_.AddItem( pNew );
    idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadLogisticSupplyClass
// Created: AGE 2007-08-21
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadLogisticSupplyClass( xml::xistream& input )
{
    std::string strName;
    int id ( 0 );
    input >> xml::attribute( "type", strName );
    input >> xml::optional >> xml::attribute( "id", id );
    helpers::T_LogisticSupplyClass_Vector::iterator found = std::find_if( vLogisticSupplyClasses_.begin(), vLogisticSupplyClasses_.end(), ADN_String_Cmp( strName ) );
    if( found != vLogisticSupplyClasses_.end() )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated resource logistic category '%1'" ).arg( strName.c_str() ).toStdString() );
    if( !id )
        id = idManager_.GetNextId();
    helpers::LogisticSupplyClass* pNew = new helpers::LogisticSupplyClass( strName, id );
    vLogisticSupplyClasses_.AddItem( pNew );
    idManager_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadDotationNatures( xml::xistream& input )
{
    input >> xml::start( "natures" )
            >> xml::list( "nature", *this, &ADN_Categories_Data::ReadNature )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::ReadLogisticSupplyClasses
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::ReadLogisticSupplyClasses( xml::xistream& input )
{
    input >> xml::start( "logistic-supply-classes" )
            >> xml::list( "logistic-supply-class", *this, &ADN_Categories_Data::ReadLogisticSupplyClass )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteSizes
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteSizes( xml::xostream& output )
{
    // Check the sizes data for duplicates.
    if( HasDuplicates( vSizes_, StringExtractor() ) )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated volume type names" ).toStdString() );

    output << xml::start( "volumes" );
    ADN_Tools::AddSchema( output, "Volumes" );
    for( T_SizeInfos_Vector::const_iterator itSize = vSizes_.begin(); itSize != vSizes_.end(); ++itSize )
    {
        if( ( *itSize )->strName_.GetData().empty() )
            throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Invalid volume type name" ).toStdString() );
        std::string strData( ( *itSize )->strName_.GetData() );
        output << xml::start( "volume" )
                << xml::attribute( "name", trim( strData ) )
               << xml::end;

    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteArmors
// Created: APE 2004-11-16
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteArmors( xml::xostream& output )
{
    // Check the armors data for duplicates.
    if( HasDuplicates( vArmors_, StringExtractor() ) )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated armor type names" ).toStdString() );

    output << xml::start( "protections" );
    ADN_Tools::AddSchema( output, "Armors" );
    for( helpers::T_ArmorInfos_Vector::const_iterator itArmor = vArmors_.begin(); itArmor != vArmors_.end(); ++itArmor )
        ( *itArmor )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteDotationNatures
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteDotationNatures( xml::xostream& output )
{
    // Check the dotation natures for duplicates.
    if( HasDuplicates( vDotationNatures_, StringExtractor() ) )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated resource nature names" ).toStdString() );

    output << xml::start( "natures" );
    ADN_Tools::AddSchema( output, "ResourceNatures" );
    for( helpers::T_ResourceNatureInfos_Vector::const_iterator it = vDotationNatures_.begin(); it != vDotationNatures_.end(); ++it )
    {
        if( ( *it )->strName_.GetData().empty() )
            throw MASA_EXCEPTION( tools::translate( "Categories_Data","Categories - Invalid resource nature" ).toStdString() );
        std::string strData( ( *it )->strName_.GetData() );
        output << xml::start( "nature" )
                << xml::attribute( "type", trim( strData ) )
                << xml::attribute( "id", ( *it )->nId_ )
               << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::WriteLogisticSupplyClasses
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_Categories_Data::WriteLogisticSupplyClasses( xml::xostream& output )
{
    // Check the dotation natures for duplicates.
    if( HasDuplicates( vLogisticSupplyClasses_, StringExtractor() ) )
        throw MASA_EXCEPTION( tools::translate( "Categories_Data", "Categories - Duplicated logistic resource category" ).toStdString() );

    output << xml::start( "logistic-supply-classes" );
    ADN_Tools::AddSchema( output, "LogisticSupplyClasses" );
    for( helpers::T_LogisticSupplyClass_Vector::const_iterator it = vLogisticSupplyClasses_.begin(); it != vLogisticSupplyClasses_.end(); ++it )
    {
        if( ( *it )->strName_.GetData().empty() )
            throw MASA_EXCEPTION( tools::translate( "Categories_Data","Categories - Invalid resource nature" ).toStdString() );
        std::string strData( ( *it )->strName_.GetData() );
        output << xml::start( "logistic-supply-class" )
                << xml::attribute( "type", trim( strData ) )
                << xml::attribute( "id", ( *it )->nId_ )
               << xml::end;
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::GetNewIdentifier
// Created: RPD 2010-11-03
// -----------------------------------------------------------------------------
unsigned long ADN_Categories_Data::GetNewIdentifier()
{
    return idManager_.GetNextId();
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_Data::CheckDatabaseValidity
// Created: LDC 2012-10-24
// -----------------------------------------------------------------------------
void ADN_Categories_Data::CheckDatabaseValidity( ADN_ConsistencyChecker& checker ) const
{
    if( vArmors_.size() == 1 )
        checker.AddError( eMissingArmor, "", eCategories );
}
