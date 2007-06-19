// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_EnumGenerator.h"
#include "AGR_Workspace.h"
#include "AGR_Enumeration.h"

// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator constructor
// Created: APE 2005-02-08
// -----------------------------------------------------------------------------
AGR_EnumGenerator::AGR_EnumGenerator()
{
}


// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator destructor
// Created: APE 2005-02-08
// -----------------------------------------------------------------------------
AGR_EnumGenerator::~AGR_EnumGenerator()
{
}

// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::Generate
// Created: APE 2005-02-08
// -----------------------------------------------------------------------------
void AGR_EnumGenerator::Generate( const AGR_Workspace& workspace, const std::string& strOutputPath )
{
    MT_MakeDir( strOutputPath + "/src" );
    MT_MakeDir( strOutputPath + "/src/libraries" );
    MT_MakeDir( strOutputPath + "/src/libraries/ENT" );
    std::cout << "Generating enums file" << std::endl;
    GenerateEnumFile( workspace, strOutputPath );
    GenerateTranslatorHeader( workspace, strOutputPath );
    GenerateTranslatorImplementation( workspace, strOutputPath );
}

// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::GenerateEnumFile
// Created: APE 2005-02-09
// -----------------------------------------------------------------------------
void AGR_EnumGenerator::GenerateEnumFile( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    // Standard enums
    std::stringstream strEnumsDeclaration;
    const T_Enumeration_Vector& enums = workspace.Enums();
    std::stringstream strContent;

    for( CIT_Enumeration_Vector it = enums.begin(); it != enums.end(); ++it )
    {
        std::string strEnumName = (*it)->GetName();
        const T_String_Vector& enumValues = (*it)->GetEnumValueSet();

        strEnumsDeclaration << "enum " << CreatePrettyEnumName( strEnumName ) << "\n{\n";

        for( CIT_String_Vector itValues = enumValues.begin(); itValues != enumValues.end(); ++itValues )
        {
            strEnumsDeclaration << "    " << CreatePrettyValueName( strEnumName, *itValues ) << ",\n"; 
        }

        strEnumsDeclaration << "    eNbr" << GetCoreEnumName( strEnumName ) << "\n};\n\n\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_ENT_Enums_Skeleton.h", strBaseContent );
    workspace.ReplaceInString( strBaseContent, "$Enums$", strEnumsDeclaration.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/libraries/ENT/ENT_Enums_Gen.h" );
}

// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::GenerateTranslatorHeader
// Created: APE 2005-02-11
// -----------------------------------------------------------------------------
void AGR_EnumGenerator::GenerateTranslatorHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::stringstream strConvertFromFunctions;
    std::stringstream strConvertToFunctions;
    std::stringstream strTypedefs;
    std::stringstream strConverters;
    const T_Enumeration_Vector& enums = workspace.Enums();

    for( CIT_Enumeration_Vector it = enums.begin(); it != enums.end(); ++it )
    {
        std::string strEnumName = (*it)->GetName();
        std::string strPrettyEnumName = CreatePrettyEnumName( strEnumName );
        std::string strCoreEnumName = GetCoreEnumName( strEnumName );

        // static E_BreakdownType ConvertToBreakdownType( const std::string& );

        strConvertToFunctions << "    static " << strPrettyEnumName << " ConvertTo" << strCoreEnumName 
                              << "( const std::string& );\n";

        // static const std::string& ConvertFromObjectType( E_ObjectType, E_Conversion = eToSim );

        strConvertFromFunctions << "    static const std::string& ConvertFrom" << strCoreEnumName
                                << "( " << strPrettyEnumName << ", E_Conversion = eToSim );\n";

        // typedef ENT_Tr::converter<E_ObjectType> T_ConverterObject;
        strTypedefs << "    typedef ENT_Tr::converter<" << strPrettyEnumName << "> T_Converter" << strCoreEnumName << ";\n";

        // static T_ConverterObject objectTypeConverter_[];
        strConverters << "    static T_Converter" << strCoreEnumName << " " << strCoreEnumName << "Converter_ [];\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_ENT_Tr_Skeleton.h", strBaseContent );
    workspace.ReplaceInString( strBaseContent, "$ConvertFromFunctions$", strConvertFromFunctions.str() );
    workspace.ReplaceInString( strBaseContent, "$ConvertToFunctions$", strConvertToFunctions.str() );
    workspace.ReplaceInString( strBaseContent, "$Typdefs$", strTypedefs.str() );
    workspace.ReplaceInString( strBaseContent, "$Converters$", strConverters.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/libraries/ENT/ENT_Tr_Gen.h" );
}


// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::GenerateTranslatorImplementation
// Created: APE 2005-02-11
// -----------------------------------------------------------------------------
void AGR_EnumGenerator::GenerateTranslatorImplementation( const AGR_Workspace& workspace, const std::string& strOutputPath ) const
{
    std::stringstream strConverters;
    std::stringstream strConvertFromFunctions;
    std::stringstream strConvertToFunctions;
    std::stringstream strInitTr;

    const T_Enumeration_Vector& enums = workspace.Enums();

    for( CIT_Enumeration_Vector it = enums.begin(); it != enums.end(); ++it )
    {
        std::string strEnumName = (*it)->GetName();
        std::string strPrettyEnumName = CreatePrettyEnumName( strEnumName );
        std::string strCoreEnumName = GetCoreEnumName( strEnumName );
        std::string strConverterType = "T_Converter" + strCoreEnumName;
        std::string strConverterName = strCoreEnumName + "Converter_";
        std::string strConvertToFctName = "ConvertTo" + strCoreEnumName;
        std::string strConvertFromFctName = "ConvertFrom" + strCoreEnumName;
        const T_String_Vector& enumValues = (*it)->GetEnumValueSet();

        strConverters << "ENT_Tr::" << strConverterType << " ENT_Tr::" << strConverterName << "[] =\n{\n";

        for( CIT_String_Vector itValue = enumValues.begin(); itValue != enumValues.end(); ++itValue )
        {
            std::string strScriptName = CreateScriptName( *itValue );
            strConverters << "    " << strConverterType << "( \"" << strScriptName << "\", QT_TRANSLATE_NOOP( \"ENT_Tr\", \"" 
                          << strScriptName << "\" ), " << CreatePrettyValueName( strEnumName, *itValue ) << " ),\n"; 
        }
        strConverters << "    " << strConverterType << "( \"\", \"\", (" << strPrettyEnumName << ")-1 )\n"
                      << "};\n\n";


        //-----------------------------------------------------------------------------
        // Name: ENT_Tr::ConvertPostureType
        // Created: AGR
        //-----------------------------------------------------------------------------
        // E_PostureType ENT_Tr::ConvertPostureType( const std::string& sName )
        // {
        //    return ADN_Tools::FindInConverter( postureConverter_, sName );
        // }
        strConvertToFunctions << "//-----------------------------------------------------------------------------\n"
                              << "// Name: ENT_Tr::" << strConvertToFctName << "\n"
                              << "// Created: AGR\n"
                              << "//-----------------------------------------------------------------------------\n"
                              << strPrettyEnumName << " ENT_Tr::" << strConvertToFctName << "( const std::string& strName )\n"
                              << "{\n"
                              << "    return ENT_Tr::FindInConverter( " << strConverterName << ", strName );\n"
                              << "}\n\n";

        //-----------------------------------------------------------------------------
        // Name: ENT_Tr::GetPostureTypeName
        // Created: JDY 03-07-24
        //-----------------------------------------------------------------------------
        // const std::string& ENT_Tr::GetPostureTypeName( E_PostureType val ,ENT_Tr_ABC::E_Conversion e)
        // {
        //    return ADN_Tools::InverseFindInConverter( postureConverter_, val ,e);
        // }
        strConvertFromFunctions << "//-----------------------------------------------------------------------------\n"
                                << "// Name: ENT_Tr::" << strConvertFromFctName << "\n"
                                << "// Created: AGR\n"
                                << "//-----------------------------------------------------------------------------\n"
                                << "const std::string& ENT_Tr::" << strConvertFromFctName << "( " << strPrettyEnumName << " nValue, ENT_Tr_ABC::E_Conversion nConverterType )\n"
                                << "{\n"
                                << "    return ENT_Tr::InverseFindInConverter( " << strConverterName << ", nValue, nConverterType );\n"
                                << "}\n\n";

        //     InitTr(carriedAmmoConverter_);
        strInitTr << "    InitTr( " << strConverterName << ", \"ENT_Tr\" );\n";
    }

    std::string strBaseContent = "";
    workspace.ReadStringFile( AGR_SKEL_DIR "AGR_ENT_Tr_Skeleton.cpp", strBaseContent );
    workspace.ReplaceInString( strBaseContent, "$ConvertFromFunctions$", strConvertFromFunctions.str() );
    workspace.ReplaceInString( strBaseContent, "$ConvertToFunctions$", strConvertToFunctions.str() );
    workspace.ReplaceInString( strBaseContent, "$Converters$", strConverters.str() );
    workspace.ReplaceInString( strBaseContent, "$InitTr$", strInitTr.str() );
    workspace.WriteStringInFile( strBaseContent, strOutputPath + "/src/libraries/ENT/ENT_Tr_Gen.cpp" );
}

// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::CreatePrettyEnumName
/*
    EnumMissionInfNatureObjectif    -> E_MissionInfNatureObjectif
    EnumAutomatOrderFormation      -> E_AutomatOrderFormation
    EnumControlErrorCode               -> E_CtrlErrorCode
    MsgCtrlClientAnnouncement       -> E_MsgCtrlClientAnnouncement
*/
// Created: APE 2005-02-10
// -----------------------------------------------------------------------------
std::string AGR_EnumGenerator::CreatePrettyEnumName( const std::string strEnumName ) const
{
    return "E_" + GetCoreEnumName( strEnumName );
}

// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::CreatePrettyValueName
/*
    enum EnumHumanRank
    {
        mdr,            -> eHumanRank_Mdr
        officier,       -> eHumanRank_Officier
        sous_officer    -> eHumanRank_SousOfficier
    }
*/
// Created: APE 2005-02-10
// -----------------------------------------------------------------------------
std::string AGR_EnumGenerator::CreatePrettyValueName( const std::string strEnumName, const std::string strValueName ) const
{
    std::string strPrettyName = "e" + GetCoreEnumName( strEnumName ) + "_";
    bool bNextCharInUpperCase = true;
    for( uint n = 0; n < strValueName.length(); ++n )
    {
        char c = strValueName[n];
        if( c == '_' )
        {
            bNextCharInUpperCase = true;
            continue;
        }
        if( bNextCharInUpperCase && c >= 'a' && c <= 'z')
        {
            bool bAllTheSameChars = true;
            for( uint i = n; i < strValueName.length(); ++i )
            {
                if( strValueName[i] != c )
                {
                    bAllTheSameChars = false;
                    break;
                }
            }
            if( ! bAllTheSameChars )
                c -= 32;
        }
        bNextCharInUpperCase = false;
        strPrettyName += c;
    }
    return strPrettyName;
}


// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::CreateScriptName
/*
    mdr,            -> mdr
    officier,       -> officier
    sous_officer    -> sous officer
*/
// Created: APE 2005-02-11
// -----------------------------------------------------------------------------
std::string AGR_EnumGenerator::CreateScriptName( const std::string strValue ) const
{
    std::string strScriptName;
    for( uint n = 0; n < strValue.length(); ++n )
    {
        if( strValue[n] == '_' )
            strScriptName += ' ';
        else
            strScriptName += strValue[n];
    }
    return strScriptName;
}


// -----------------------------------------------------------------------------
// Name: AGR_EnumGenerator::GetCoreEnumName
/*
    EnumMissionInfNatureObjectif    -> MissionInfNatureObjectif
    EnumAutomatOrderFormation      -> AutomatOrderFormation
    EnumControlErrorCode               -> CtrlErrorCode
    MsgCtrlClientAnnouncement       -> MsgCtrlClientAnnouncement
*/
// Created: APE 2005-02-10
// -----------------------------------------------------------------------------
std::string AGR_EnumGenerator::GetCoreEnumName( const std::string strEnumName ) const
{
    if( strEnumName.length() <= 4 )
        return strEnumName;

    if( strEnumName.substr( 0, 4 ) == "Enum" )
        return strEnumName.substr( 4, strEnumName.length() - 4 );
    else
        return strEnumName;
}


