// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-07 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "AGR_pch.h"

#include "AGR_TypeRC.h"
#include "AGR_Workspace.h"
#include "AGR_Type_ABC.h"
#include "AGR_RC.h"
#include <boost/algorithm/string.hpp>

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::Param
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_TypeRC::Param::Param( MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
    : strName_  ( )
    , pType_    ()
    , bOptional_( false )
{
    input.Section( "xsd:element" );
    std::string strParamType;
    input.ReadAttribute( "name", strName_ );
    input.ReadAttribute( "type", strParamType );

    pType_ = workspace.FindType( strParamType );
    if( !pType_ )
        throw std::exception( "Unable to determine type for RC" );

    uint nTmp;
    input.EnableExceptions( false );
    bool bOptional = input.ReadAttribute( "minOccurs", nTmp );
    if( !bOptional )
        input.ClearLastError();
    input.EnableExceptions( true );

    input.EndSection(); // xsd:element
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC constructor
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_TypeRC::AGR_TypeRC( const std::string& strName, MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
    : strAsnName_    ( strName )
    , strName_       ()
    , strMILEnumName_( )
    , strHumanName_  ()
    , pType_         ( 0 )
{
    strName_        = strAsnName_.substr( 3 );
    strMILEnumName_ = "eRC_" + strName_;

    strHumanName_ = strName_;
    std::string::iterator prevIt = strHumanName_.begin();
    for( std::string::iterator it = strHumanName_.begin(); it != strHumanName_.end(); )
    {
        std::string::iterator nextIt = it;
        ++ nextIt;
        if( it != strHumanName_.begin() )
        {
            prevIt = it;
            --prevIt;
        }
        if(    ( it != strHumanName_.begin() && nextIt != strHumanName_.end() && boost::is_upper()( *it ) && boost::is_lower()( *nextIt ) )
            || ( boost::is_upper()( *it ) && boost::is_lower() ( *prevIt ) ) )
        {
            it = strHumanName_.insert( it, ' ' );
            ++it;
            ++it;
            
        }
        else
        {
            ++it;
        }
    }
    boost::to_lower( strHumanName_ );

    std::string strSubSection = input.GetCurrentElementName();
    if( strSubSection == "xsd:restriction" )
    {
        input.Section( "xsd:restriction" );
        std::string strType;
        input.ReadAttribute( "base", strType );
        pType_ = workspace.FindType( strType );
        if( !pType_ )
            throw std::exception( "Unable to determine type for RC" );
        input.EndSection(); // "xsd:restriction"a
    }
    else if( strSubSection == "xsd:sequence" )
    {
        input.BeginList( "xsd:sequence" );
        while( input.NextListElement() )
        {            
            Param* pParam = new Param( input, workspace );
            params_.push_back( pParam );
        }
        input.EndList(); // xsd:sequence
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC destructor
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_TypeRC::~AGR_TypeRC()
{

}

// =============================================================================
// GENERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateEnum
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateEnum() const
{
    std::stringstream strTmp;

    strTmp << strMILEnumName_
           << ",";

    if( pType_ )
    {
        assert( params_.empty() );
        strTmp <<  " // Parametre : "
               << pType_->GetDIAType();
    }
    else if( !params_.empty() )
    {
        if( params_.size() > 1 )
            strTmp <<  " // Parametres : ";
        else
            strTmp <<  " // Parametre : ";
        for( CIT_ParamVector it = params_.begin(); it != params_.end(); ++it )
        {
            strTmp << (**it).strName_
                   << " (" << (**it).pType_->GetDIAType() << ")";
            if( it != params_.rbegin().base() )
                strTmp << ", ";
        }
    }
    strTmp << std::endl;
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateHALEnum
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateHALEnum( const AGR_RC& /*rcInstance*/ ) const
{
    return GenerateEnum();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMILRCClassHeader
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMILRCClassHeader( const AGR_RC& /*rcInstance*/ ) const
{
    return GenerateEnum();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMILRCClassBodyInitialize
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMILRCClassBodyInitialize( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "rcs_[ " << strMILEnumName_ << " ] = new ";

    if( pType_ || !params_.empty() )
        strTmp << "MIL_RC_" << strName_;
    else
        strTmp << "MIL_RC";

    strTmp << "( " 
                << strMILEnumName_ 
                << ", "
                << "T_MsgCR_cr_" << rcInstance.GetAsnName()
           << " );";

    strTmp << std::endl;
    return strTmp.str();

}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMILRCClassBodyIncludes
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMILRCClassBodyIncludes( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;
    if( pType_ || !params_.empty() )
        strTmp << "#include \"MIL_RC_" << strName_ << ".h\"" << std::endl;

    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMOS2
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMOS2( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "case T_MsgCR_cr_" << rcInstance.GetAsnName() 
           << " : strMsg << \"" << strHumanName_ << "\"";


    if( pType_ )
    {
        assert( params_.empty() );
        strTmp << " << \" - " << pType_->GetHumanName() << " : \""
               << " << \" \" << "
               << pType_->MOS2RCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() );

    }
    else if( !params_.empty() )
    {
        for( CIT_ParamVector it = params_.begin(); it != params_.end(); ++it )
        {
            strTmp << " << \" - "
                   << (**it).strName_
                   << " : "
                   << "\" << "
                   << (**it).pType_->MOS2RCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() + "->" + (**it).strName_ );        
        }

    }

    strTmp << "; break;" << std::endl;
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMOS
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMOS( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "case T_MsgCR_cr_" << rcInstance.GetAsnName() 
           << " : strMsg << \"" << strHumanName_ << "\"";


    if( pType_ )
    {
        assert( params_.empty() );
        strTmp << " << \" - " << pType_->GetHumanName() << " : \""
               << " << \" \" << "
               << pType_->MOSRCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() );

    }
    else if( !params_.empty() )
    {
        for( CIT_ParamVector it = params_.begin(); it != params_.end(); ++it )
        {
            strTmp << " << \" - "
                   << (**it).strName_
                   << " : "
                   << "\" << "
                   << (**it).pType_->MOSRCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() + "->" + (**it).strName_ );        
        }

    }

    strTmp << "; break;" << std::endl;
    return strTmp.str();
}
