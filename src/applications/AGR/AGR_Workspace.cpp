// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Workspace.cpp $
// $Author: Nld $
// $Modtime: 31/03/05 10:05 $
// $Revision: 29 $
// $Workfile: AGR_Workspace.cpp $
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_Workspace.h"

#include "AGR_Mission.h"
#include "AGR_Enumeration.h"
#include "AGR_Member.h"
#include "AGR_Type_ABC.h"
#include "AGR_FragOrder.h"
#include "AGR_TypeRC.h"
#include "AGR_RC.h"

#include <windows.h>

#include <assert.h>

#include "AGR_Types.h"

// -----------------------------------------------------------------------------
// Name: AGR_Workspace constructor
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
AGR_Workspace::AGR_Workspace()
: bMsgCRParsed_( false )
{
    parsedFileSet_.insert( std::string( "osxsdlib.xsd" ) );

    // 1st boolean : bRequiresCleaning (asn)
    // 2nd boolean : bRequiresReset

    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:ListAgent", "AgentList", "T_ListePions", "T_IdVector", true ) );
    typeList_.push_back( new AGR_AgentType() );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:ListAutomate", "AutomateList", "T_ListeAutomates", "T_IdVector", true ) );
    typeList_.push_back( new AGR_AutomateType() );
    typeList_.push_back( new AGR_Type_ABC( "xsd:boolean", "Bool", "bool", "bool", false ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:Direction", "Direction", "T_Direction", "T_Direction", false ) );
    typeList_.push_back( new AGR_Type_ABC( "xsd:double", "Numeric", "float", "double", false ) );
    typeList_.push_back( new AGR_GDHType() );
    typeList_.push_back( new AGR_Type_ABC( "ListMissionGenObject", "GenObjectList", "T_ListeGenObjets", "T_GenObjectVector", true, true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioAutomateMission_Genie:MissionGenObject", "GenObject", "T_GenObjet", "GenObject", true, true ) );
    typeList_.push_back( new AGR_IntegerType() );
    typeList_.push_back( new AGR_IntegerType( "Pourcentage" ) );
    typeList_.push_back( new AGR_IntegerType( "PourcentageLibre" ) );
    typeList_.push_back( new AGR_IntegerType( "Hauteur" ) );
    typeList_.push_back( new AGR_IntegerType( "Altitude" ) );
    typeList_.push_back( new AGR_IntegerType( "Vitesse" ) );
    typeList_.push_back( new AGR_IntegerType( "Angle", 0, 360 ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:Itineraire", "Path", "T_ListePoints", "Path", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:ListItineraire", "PathList", "T_ListeListesPoints", "T_PathVector", true ) );
    typeList_.push_back( new AGR_KnowledgeAgentListType() );
    typeList_.push_back( new AGR_KnowledgeAgentType() );
    typeList_.push_back( new AGR_KnowledgeObjectListType() );
    typeList_.push_back( new AGR_KnowledgeObjectType() );
    typeList_.push_back( new AGR_KnowledgePopulationType() );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:Line", "Location", "T_Localisation", "Location", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:ListLocalisation", "LocationList", "T_ListeLocalisations", "T_LocationVector", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:ListPoint", "PointList", "T_ListePoints", "T_PositionVector", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:Localisation", "Location", "T_Localisation", "Location", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:NatureAtlas", "NatureAtlas", "E_NatureAtlasType", "uint", false ) );
    typeList_.push_back( new AGR_OIDType() );
    typeList_.push_back( new AGR_PointType() );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:ListPolygon", "PolygonList", "T_ListeLocalisations", "T_LocationVector", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:MaintenancePriorites", "MaintenancePriorities", "T_MaintenancePriorites", "", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:SantePriorites", "MedicalPriorities", "T_SantePriorites", "T_IdVector", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:TypeEquipement", "TypeEquipement", "TypeEquipement", "TypeEquipement", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:TypeDotation", "TypeDotation", "TypeDotation", "TypeDotation", true ) );
    typeList_.push_back( new AGR_Type_ABC( "ScipioBase:TirPion", "TirPion", "TirPion", "TirPion", true ) );
    typeList_.push_back( new AGR_Type_ABC( "xsd::string", "message", "message", "message", true ) );

    typeList_.push_back( new AGR_PolygonType() );
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace destructor
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
AGR_Workspace::~AGR_Workspace()
{
    MT_DELETEOWNED( missionList_ );
    MT_DELETEOWNED( typeList_ );
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Read
/** @param  strFile 
*/
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Workspace::Read( const std::string& strFile )
{
    std::string strModuleName;
    MT_ExtractBaseName( strFile, strModuleName );

    try
    {
        MT_XXmlInputArchive input;
        input.Open( "./agr_tmp/" + strFile );
        input.EnableExceptions( true );
        Read( input, strModuleName + ":" );
    }
    catch( MT_Exception& e )
    {
        std::stringstream strMsg;
        strMsg << e.GetWholeMessage();

        MessageBox( 0, strMsg.str().c_str(), "ASN Class Generator - Error while parsing files", MB_ICONERROR | MB_OK );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Read
/** @param  input 
*/
// Created: AGN 2004-04-22
// -----------------------------------------------------------------------------
void AGR_Workspace::Read( MT_XXmlInputArchive& input, const std::string& strModuleName )
{
    input.BeginList( "xsd:schema" );

    while( input.NextListElement() )
    {
        std::string strSection = input.GetCurrentElementName();

        if( strSection == "xsd:import" )
            ReadImport( input, strModuleName );
        else if( strSection == "xsd:simpleType" )
            ReadSimpleType( input, strModuleName );
        else if( strSection == "xsd:complexType" )
            ReadComplexType( input, strModuleName );
        else
        {
            // Force the list pointer to advance.
            input.Section( strSection );
            input.EndSection();
        }
    }    
    input.EndList(); // xsd:schema


    // 2nd pass on low names. Should be useless on fresh and well formed asns.
    for( CIT_StringMap it = lowNameMap_.begin(); it != lowNameMap_.end(); ++it )
    {
        AGR_Class* pClass = FindMission( it->first );
        if( ! pClass )
            pClass = FindFragOrder( it->first );
        if( pClass )
            pClass->SetLowName( it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadImport
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadImport( MT_XXmlInputArchive& input, const std::string& /*strModuleName*/ )
{
    input.Section( "xsd:import" );
    // read an other file
    std::string strFileName;
    input.ReadAttribute( "schemaLocation", strFileName );

    if( parsedFileSet_.find( strFileName ) == parsedFileSet_.end() )
    {
        Read( strFileName );
        parsedFileSet_.insert( strFileName );
    }

    input.EndSection(); // xsd:import
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadSimpleType
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadSimpleType( MT_XXmlInputArchive& input, const std::string& strModuleName )
{
    input.BeginList( "xsd:simpleType" );
    std::string strTypeName;
    input.ReadAttribute( "name", strTypeName );

    // FRAGMENTARY ORDER
    if( strTypeName.substr( 0, 14 ) == "OrderConduite_" )
    {
        // this is a simple type whose name starts with OrderConduite_ --> define a order
        // Some items can be read multiple times because of file inclusions. Ensure that we read them only once.
        if( FindFragOrder( strTypeName ) == 0 )
        {
            AGR_FragOrder* pFragOrder( new AGR_FragOrder( strTypeName, strModuleName ) );
            pFragOrder->Read( input, *this );
            fragOrderList_.push_back( pFragOrder );
        }
        
        input.EndList(); // "xsd:simpleType"
        return;
    }

    // RC
    else if( strTypeName.substr( 0, 3 ) == "CR_" )
    {
        if( typeRCs_.find( strTypeName ) == typeRCs_.end() )
        {
            const AGR_TypeRC* pRC = new AGR_TypeRC( strTypeName, input, *this );
            typeRCs_.insert( std::make_pair( strTypeName, pRC ) ).second;
        }
    }

    // POSSIBLY AN ENUM
    std::string strSubSection = input.GetCurrentElementName();
    if( strSubSection == "xsd:restriction" )
    {
        input.Section( "xsd:restriction" );
        std::string strTypeBase;
        input.ReadAttribute( "base", strTypeBase );

        // AN ENUM
        if( strTypeBase == "xsd:token" )
        {
            // Some items can be read multiple times because of file inclusions. Ensure that we read them only once.
            bool bFound = false;
            for( IT_Enumeration_Vector it = enumerationList_.begin(); it != enumerationList_.end(); ++it )
            {
                if( (*it)->GetName() == strTypeName )
                    bFound = true;
            }

            if( ! bFound )
            {
                // we define an enumeration. Just mark it as enum
                AGR_Enumeration* pEnum = new AGR_Enumeration( strTypeName );
                pEnum->Read( input );
                typeList_.push_back( pEnum );
                enumerationList_.push_back( pEnum );
            }
        }

        input.EndSection(); // "xsd:restriction"
    }

    input.EndList(); // "xsd:simpleType"
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadComplexType
// Created: AGE 2004-09-15
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadComplexType( MT_XXmlInputArchive& input, const std::string& strModuleName )
{
    input.BeginList( "xsd:complexType" );
    std::string strTypeName;
    input.ReadAttribute( "name", strTypeName );

    // POSSIBLY A MISSION ENUMERATION
    std::string strSubString = input.GetCurrentElementName();
    if( strSubString == "xsd:choice" )
    {
        input.Section( "xsd:choice" );

        // A MISSION ENUMERATION
        if( strTypeName.substr( 0, 8 ) == "Mission_" )
        {
            // this should define the lower name for the missions
            while( input.NextListElement() )
            {
                input.Section( "xsd:element" );
                std::string strLowName;
                std::string strMissionName;
                input.ReadAttribute( "name", strLowName );
                input.ReadAttribute( "type", strMissionName );
                AGR_Mission* pMission = FindMission( strMissionName );
                if( pMission != 0 )
                    pMission->SetLowName( strLowName );
                else
                    lowNameMap_.insert( std::make_pair( strMissionName, strLowName ) );
                input.EndSection(); // xsd:element
            }                          
        }
        input.EndSection(); // xsd:choice
        input.EndList(); // xsd:complexType
        return;
    }

    // MsgAttenteOrderConduite
  /* else if( strTypeName == "MsgAttenteOrdreConduite" )
    {
        input.Section( "xsd:sequence" );
        while( input.NextListElement() )
        {
            input.Section( "xsd:element" );
            std::string strTypeName2;
            input.ReadAttribute( "name", strTypeName2 );
            if( strTypeName2 == "ordres_conduite" )
            {
                input.Section( "xsd:complexType" );
                input.Section( "xsd:sequence" );
                input.Section( "xsd:element" );
                input.Section( "xsd:complexType" );
                input.Section( "xsd:choice" );
                // This should define the lower name for the orders demands
                while( input.NextListElement() )
                {
                    input.Section( "xsd:element" );
                    std::string strLowName;
                    std::string strDemandName;
                    input.ReadAttribute( "name", strLowName );
                    input.ReadAttribute( "type", strDemandName );
                    std::string strOrderName = strDemandName.substr( std::string( "Demande" ).size() );
                    AGR_FragOrder* pFragOrder = FindFragOrder( strOrderName );
                    if( pFragOrder != 0 )
                        pFragOrder->SetDemandLowName( strLowName );
                    else
                        demandLowNameMap_.insert( std::make_pair( strOrderName, strLowName ) );
                    input.EndSection(); // xsd:element
                }
                input.EndSection(); // "xsd:choice"
                input.EndSection(); // "xsd:complexType"
                input.EndSection(); // "xsd:element"
                input.EndSection(); // "xsd:seqence"
                input.EndSection(); // "xsd:complexType"
            }
            input.EndSection(); // "xsd:element"
        }
        input.EndSection(); // "xsd:sequence"
        input.EndList(); // "xsd:complexType"
        return;
    }*/

    // A FRAGMETARY ORDER ENUMERATION
    else if( strTypeName == "MsgOrderConduite" )
    {
        input.Section( "xsd:sequence" );
        while( input.NextListElement() )
        {
            input.Section( "xsd:element" );
            std::string strTypeName2;
            input.ReadAttribute( "name", strTypeName2 );
            if( strTypeName2 == "order_conduite" )
            {
                input.Section( "xsd:complexType" );
                input.Section( "xsd:choice" );
                // This should define the lower name for the orders
                while( input.NextListElement() )
                {
                    input.Section( "xsd:element" );
                    std::string strLowName;
                    std::string strMissionName;
                    input.ReadAttribute( "name", strLowName );
                    input.ReadAttribute( "type", strMissionName );
                    AGR_FragOrder* pFragOrder = FindFragOrder( strMissionName );
                    if( pFragOrder != 0 )
                        pFragOrder->SetLowName( strLowName );
                    else
                        lowNameMap_.insert( std::make_pair( strMissionName, strLowName ) );
                    input.EndSection(); // xsd:element
                }
                input.EndSection(); // "xsd:choice"
                input.EndSection(); // "xsd:complexType"
            }
            input.EndSection(); // "xsd:element"
        }
        input.EndSection(); // "xsd:sequence"
        input.EndList(); // "xsd:complexType"
        return;
    }

    // A MISSION
    else if( strTypeName.substr( 0, 8 ) == "Mission_" )
    {
        // this is a complex type whose name starts with Mission_ --> define a mission
        // Some items can be read multiple times because of file inclusions. Ensure that we read them only once.
        if( FindMission( strModuleName + strTypeName ) == 0 )
        {
            AGR_Mission* pNewMission( new AGR_Mission( strTypeName, strModuleName ) );
            pNewMission->Read( input, *this );
            missionList_.push_back( pNewMission );
        }
        input.EndList(); // xsd:complexType
        return;
    }

    // A FRAGMENTARY ORDER
    else if( strTypeName.substr( 0, 14 ) == "OrderConduite_" )
    {
        // this is a complex type whose name starts with OrderConduite_ --> define a order
        // Some items can be read multiple times because of file inclusions. Ensure that we read them only once.
        if( FindFragOrder( strTypeName ) == 0 )
        {
            AGR_FragOrder* pFragOrder( new AGR_FragOrder( strTypeName, strModuleName ) );
            pFragOrder->Read( input, *this );
            fragOrderList_.push_back( pFragOrder );
        }
        input.EndList(); // xsd:complexType
        return;
    }

    // RC Type
    else if( strTypeName.substr( 0, 3 ) == "CR_" )
    {
        if( typeRCs_.find( strTypeName ) == typeRCs_.end() )
        {
            const AGR_TypeRC* pRC = new AGR_TypeRC( strTypeName, input, *this );
            typeRCs_.insert( std::make_pair( strTypeName, pRC ) ).second;
        }
    }

    // RC list
    else if( strTypeName == "MsgCR" && !bMsgCRParsed_ )
    {
        bMsgCRParsed_ =  true; /// $$$ n'importe quoi
        input.BeginList( "xsd:sequence" );
        while( input.NextListElement() )
        {
            input.Section( "xsd:element" );
            std::string strTmp;
            input.ReadAttribute( "name", strTmp );
            if( strTmp == "cr" )
            {
                input.Section( "xsd:complexType" );
                input.BeginList( "xsd:choice" );
                while( input.NextListElement() )
                {
                    AGR_RC* pRC = new AGR_RC( input, *this );
                    rcs_.push_back( pRC );
                }
                input.EndList(); // xsd:choice
                input.EndSection(); //  xsd:complextype
            }
            input.EndSection(); // xsd:element
        }
        input.EndList(); // xsd:sequence
    }
    
    input.EndList(); // xsd:complexType
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Missions
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
const AGR_Workspace::T_Mission_Vector& AGR_Workspace::Missions() const
{
    return missionList_;    
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::FragOrders
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
const AGR_Workspace::T_FragOrder_Vector& AGR_Workspace::FragOrders() const
{
    return fragOrderList_;
}


// -----------------------------------------------------------------------------
// Name: AGR_Workspace::Enums
// Created: APE 2005-02-08
// -----------------------------------------------------------------------------
const T_Enumeration_Vector& AGR_Workspace::Enums() const
{
    return enumerationList_;
}


// -----------------------------------------------------------------------------
// Name: AGR_Workspace::FindMission
/** @param  strFullName 
    @return 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
AGR_Mission* AGR_Workspace::FindMission( const std::string& strFullName ) const
{
    for( CIT_Mission_Vector it = missionList_.begin(); it != missionList_.end(); ++it )
    {
        if( (*it)->CompleteName() == strFullName )
            return *it;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::FindFragOrder
// Created: APE 2004-05-14
// -----------------------------------------------------------------------------
AGR_FragOrder* AGR_Workspace::FindFragOrder( const std::string& strName ) const
{
    for( CIT_FragOrder_Vector it = fragOrderList_.begin(); it != fragOrderList_.end(); ++it )
    {
        if( (*it)->Name() == strName )
            return *it;
    }
    return 0;
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReplaceInString
/** @param  strToProcess 
    @param  strToSearch 
    @param  strReplace 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
void AGR_Workspace::ReplaceInString( std::string& strToProcess, const std::string& strToSearch, const std::string& strReplace ) const
{
    const std::string::size_type nSize = strToSearch.size();
    std::string::size_type nPos = 0;

    std::string::size_type newPos = strToProcess.find( strToSearch, nPos );
    while( newPos != std::string::npos )
    {
        strToProcess.replace( newPos, nSize, strReplace );
        nPos = newPos + nSize;
        newPos = strToProcess.find( strToSearch, nPos );
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::ReadStringFile
/** @param  strFileName 
    @param  strResult 
    @return 
*/
// Created: AGN 2004-04-23
// -----------------------------------------------------------------------------
void AGR_Workspace::ReadStringFile( const std::string& strFileName, std::string& strResult ) const
{
    std::ifstream ifs( strFileName.c_str() );
    if( !ifs )
        throw std::exception( std::string( "Error opening " + strFileName + " for reading." ).c_str() );
    strResult = std::string( std::istreambuf_iterator<char>( ifs ), std::istreambuf_iterator<char>() );
}

#include <direct.h>

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::WriteStringInFile
/** @param  strFileName 
    @param  strToWrite 
*/
// Created: AGN 2004-04-28
// -----------------------------------------------------------------------------
void AGR_Workspace::WriteStringInFile( const std::string& strToWrite, const std::string& strFileName ) const
{
    std::ofstream ofs( strFileName.c_str() );
    if( ! ofs.is_open() )
        throw std::exception( std::string( "Error opening " + strFileName + " for writing." ).c_str() );
    ofs << strToWrite;
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::CreateMember
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_Member* AGR_Workspace::CreateMember( const std::string& strName, const std::string& strASNType, const AGR_Class& ownerClass, bool bOptional ) const
{
    const AGR_Type_ABC* pType = FindType( strASNType );
    if( pType )
        return new AGR_Member( strName, *pType, ownerClass, bOptional );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::FindType
// Created: AGE 2004-09-14
// -----------------------------------------------------------------------------
const AGR_Type_ABC* AGR_Workspace::FindType( const std::string& strName ) const
{
    for( CIT_TypeVector it = typeList_.begin(); it != typeList_.end(); ++it )
    {
        assert( *it );
        const AGR_Type_ABC& type = **it;
        if( type.IsOfType( strName ) )
            return &type;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AGR_Workspace::CreateLocalTypeMember
// Created: AGE 2004-09-13
// -----------------------------------------------------------------------------
AGR_Member* AGR_Workspace::CreateLocalTypeMember( const std::string& strMissionName, const std::string& strMemberName, MT_XXmlInputArchive& input, const AGR_Class& ownerClass, bool bOptional ) const
{
    const AGR_Type_ABC* pType = 0;
    const std::string strSubSection = input.GetCurrentElementName();
    if( strSubSection == "xsd:simpleType" )
    {
        // simple type.
        input.BeginList( "xsd:simpleType" );
        while( input.NextListElement() )
        {
            // --> is it an enumeration ?
            std::string strSubSection2 = input.GetCurrentElementName();
            if( strSubSection2 == "xsd:restriction" )
            {
                input.Section( "xsd:restriction" );
                std::string strTypeBase;
                input.ReadAttribute( "base", strTypeBase );

                if( strTypeBase == "xsd:token" )
                {
                    AGR_Enumeration* pEnum = new AGR_Enumeration( strMissionName + "_" + strMemberName );
                    pEnum->Read( input );
                    pType = pEnum;
                }
                else
                if( strTypeBase == "xsd:unsignedByte" || strTypeBase == "xsd:unsignedShort" )
                    pType = FindType( "xsd:integer" );

                input.EndSection(); // "xsd:restriction"
            }
            else
            {
                // Force the list cursor to advance.
                input.Section( strSubSection2 );
                input.EndSection();
            }
        }

        input.EndList(); // "xsd:simpleType"
    }
    if( pType )
        return new AGR_Member( strMemberName, *pType, ownerClass, bOptional );
    return 0;
}
