// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: JVT 2004-08-23 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_InputArchive_Logger.inl $
// $Author: Ape $
// $Modtime: 22/04/05 16:34 $
// $Revision: 5 $
// $Workfile: MT_InputArchive_Logger.inl $
//
// *****************************************************************************

#include <iomanip>

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::MT_InputArchive_Logger
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
MT_InputArchive_Logger<Archive>::MT_InputArchive_Logger()
    : archive_		 ()
    , contexts_		 ()
    , warningStreams_()
{
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::MT_InputArchive_Logger<Archive>::~MT_InputArchive_Logger
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
MT_InputArchive_Logger<Archive>::~MT_InputArchive_Logger()
{
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::AddWarningStream
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_InputArchive_Logger<Archive>::AddWarningStream( std::ostream& stream )
{
    warningStreams_.insert( &stream );
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::RemoveWarningStream
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_InputArchive_Logger<Archive>::RemoveWarningStream( std::ostream& stream )
{
    warningStreams_.erase( &stream );
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::FormatContext
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_InputArchive_Logger<Archive>::FormatContext( std::ostream& stream ) const
{
    unsigned int nTab = 0;

    for ( CIT_ContextList itContext = contexts_.begin(); itContext != contexts_.end(); ++itContext )
    {
        stream << std::setw( ++nTab ) << '<' << itContext->sName_;

        const sContext::T_AttributesList& attributes = itContext->lAttributes_;
        for ( sContext::CIT_AttributesList itAttribute = attributes.begin(); itAttribute != attributes.end(); ++itAttribute )
            stream << ' ' << *itAttribute;
        stream << '>' << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::DoWarning
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_InputArchive_Logger<Archive>::DoWarning( const std::string& strTitle ) const
{
    for ( CIT_WarningStreamSet it = warningStreams_.begin(); it != warningStreams_.end(); ++it )
    {
        std::ostream& stream = **it;
        
        stream << "**** PARSER INFO ***************************************************************" << std::endl
               << "Unable to " << strTitle << std::endl
               << "Context :" << std::endl;
        FormatContext( stream );
        stream << "********************************************************************************" << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::GetContext
// Created: JVT 2004-10-19
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_InputArchive_Logger<Archive>::GetContext( std::string& strContext ) const
{
    std::stringstream context;
    FormatContext( context );
    strContext = context.str();
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::GetContext
// Created: JVT 2004-10-19
// -----------------------------------------------------------------------------
template< typename Archive >
inline
std::string MT_InputArchive_Logger<Archive>::GetContext() const
{
    std::string strContext;
    GetContext( strContext );
    return strContext;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::AddWarningStream
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
void MT_InputArchive_Logger<Archive>::ProcessError( const std::string& strTitle, ErrorPolicyType nErrorPolicy )
{
    if ( nErrorPolicy == eNothing )
    {
        archive_.ClearLastError();
        return;
    }

    if ( nErrorPolicy == eWarning )
    {
        archive_.ClearLastError();
        DoWarning( strTitle );
        return;
    }

    assert( nErrorPolicy == eThrow );

    std::stringstream strContext;
    strContext << "Unable to " << strTitle << std::endl
               << "Context :" << std::endl;
    FormatContext( strContext );
    throw MT_ArchiveLogger_Exception( strContext.str() );
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ProcessCheckError
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template<typename Archive>
template<typename T>
inline
void MT_InputArchive_Logger<Archive>::ProcessCheckError( const T& value, const std::string& currentAction, ErrorPolicyType nPolicy )
{
    std::stringstream strOperation;
    strOperation << "check value (" << value << currentAction << ')';
    ProcessError( strOperation.str(), nPolicy );
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::Open
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
bool MT_InputArchive_Logger<Archive>::Open( const std::string& fileName, ErrorPolicyType nErrorPolicy )
{
    assert( contexts_.empty() );

    if ( archive_.Open( fileName ) )
    {
        contexts_.push_back( sContext() );
        contexts_.back().sName_ = std::string( "File : " );
        contexts_.back().sName_ += fileName;
        return true;
    }
    ProcessError( std::string( "open file " ) += fileName, nErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::SetData
// Created: APE 2005-04-22
// -----------------------------------------------------------------------------
template< typename Archive >
void MT_InputArchive_Logger<Archive>::SetData( const std::string& strData )
{
    archive_.SetData( strData );
    contexts_.push_back( sContext() );
    contexts_.back().sName_ = std::string( "File : Internal application data" );
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::Close
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
bool MT_InputArchive_Logger<Archive>::Close( ErrorPolicyType nErrorPolicy )
{
    if ( archive_.Close() )
    {
        contexts_.clear();
        return true;
    }
    ProcessError( std::string( "close file" ), nErrorPolicy );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::Section
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
bool MT_InputArchive_Logger<Archive>::Section( const std::string& strSection, ErrorPolicyType nErrorPolicy )
{
    if ( archive_.Section( strSection ) )
    {
        contexts_.push_back( sContext() );
        contexts_.back().sName_ = strSection;
        return true;
    }
    ProcessError( std::string( "open section " ) += strSection, nErrorPolicy );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::EndSection
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
bool MT_InputArchive_Logger<Archive>::EndSection( ErrorPolicyType nErrorPolicy )
{
    if ( archive_.EndSection() )
    {
        contexts_.pop_back();
        return true;
    }
    ProcessError( std::string( "close section " ) += contexts_.back().sName_, nErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadField
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T, typename checker>
bool MT_InputArchive_Logger<Archive>::ReadField( const std::string& strField, T& value, const checker& checker, ErrorPolicyType nCheckErrorPolicy, ErrorPolicyType nReadErrorPolicy )
{
    if ( archive_.ReadField( strField, value ) )
    {
        if ( !checker( value ) )
        {
            ProcessCheckError( value, checker.Message(), nCheckErrorPolicy );
            return false;
        }
        return true;
    }

    ProcessError( std::string( "read field " ) += strField, nReadErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadField
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::ReadField( const std::string& strField, T& value, ErrorPolicyType nErrorPolicy )
{
    if ( archive_.ReadField( strField, value ) )
        return true;

    ProcessError( std::string( "read field " ) += strField, nErrorPolicy );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadAttribute
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T, typename checker>
bool MT_InputArchive_Logger<Archive>::ReadAttribute( const std::string& strAttribute, T& value, const checker& checker, ErrorPolicyType nCheckErrorPolicy, ErrorPolicyType nReadErrorPolicy )
{
    if ( archive_.ReadAttribute( strAttribute, value ) )
    {
        std::stringstream strFormatAttr;
        strFormatAttr << strAttribute << "=\"" << value << '\"';
        contexts_.back().lAttributes_.push_back( strFormatAttr.str() );
        if ( !checker( value ) )
        {
            ProcessCheckError( value, checker.Message(), nCheckErrorPolicy );
            return false;
        }
        return true;
    }
    ProcessError( std::string( "read attribute " ) += strAttribute, nReadErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadAttribute
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::ReadAttribute( const std::string& strAttribute, T& value, ErrorPolicyType nErrorPolicy )
{
    if( archive_.ReadAttribute( strAttribute, value ) )
    {
        std::stringstream strFormatAttr;
        strFormatAttr << strAttribute << "=\"" << value << '\"';
        contexts_.back().lAttributes_.push_back( strFormatAttr.str() );
        return true;
    }
    ProcessError( std::string( "read attribute " ) += strAttribute, nErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::Read
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T, typename checker>
bool MT_InputArchive_Logger<Archive>::Read( T& value, const checker& checker, ErrorPolicyType nCheckErrorPolicy, ErrorPolicyType nReadErrorPolicy )
{
    if ( archive_.Read( value ) )
    {
        if ( !checker( value ) )
        {            
            ProcessCheckError( value, checker.Message(), nCheckErrorPolicy );
            return false;
        }
        return true;
    }
    ProcessError( std::string( "read section value " ), nReadErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::Read
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::Read( T& value, ErrorPolicyType nErrorPolicy )
{
    if ( archive_.Read( value ) )
        return true;
    ProcessError( std::string( "read section value " ), nErrorPolicy );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::TransformTimeValue
// Created: NLD 2005-03-08
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::TransformTimeValue( const std::string& strValue, T& rTimeValue ) const
{
    if( strValue.empty() )
        return false;

    const char timeUnit = *strValue.rbegin();

    std::string       strTimeValue = strValue.substr( 0, strValue.size() - 1 );
    std::stringstream strTmp( strTimeValue );
    strTmp >> rTimeValue;

    switch( timeUnit )
    {
        case 's': break; 
        case 'm': rTimeValue *= (T)60  ; break;
        case 'h': rTimeValue *= (T)3600; break;
        default:            
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadTimeField
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T, typename checker>
bool MT_InputArchive_Logger<Archive>::ReadTimeField( const std::string& strField, T& value, const checker& checker, ErrorPolicyType nCheckErrorPolicy, ErrorPolicyType nReadErrorPolicy )
{
    std::string strTmp;
    if ( archive_.ReadField( strField, strTmp ) && TransformTimeValue( strTmp, value ) )
    {
        if ( !checker( value ) )
        {
            ProcessCheckError( value, checker.Message(), nCheckErrorPolicy );
            return false;
        }
        return true;
    }

    ProcessError( std::string( "read field " ) += strField, nReadErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadTimeField
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::ReadTimeField( const std::string& strField, T& value, ErrorPolicyType nErrorPolicy )
{
    std::string strTmp;
    if ( archive_.ReadField( strField, strTmp ) && TransformTimeValue( strTmp, value ) )
        return true;

    ProcessError( std::string( "read field " ) += strField, nErrorPolicy );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadTimeAttribute
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T, typename checker>
bool MT_InputArchive_Logger<Archive>::ReadTimeAttribute( const std::string& strAttribute, T& value, const checker& checker, ErrorPolicyType nCheckErrorPolicy, ErrorPolicyType nReadErrorPolicy )
{
    std::string strTmp;
    if ( archive_.ReadAttribute( strAttribute, strTmp ) && TransformTimeValue( strTmp, value ) )
    {
        std::stringstream strFormatAttr;
        strFormatAttr << strAttribute << "=\"" << value << '\"';
        contexts_.back().lAttributes_.push_back( strFormatAttr.str() );
        if ( !checker( value ) )
        {
            ProcessCheckError( value, checker.Message(), nCheckErrorPolicy );
            return false;
        }
        return true;
    }
    ProcessError( std::string( "read attribute " ) += strAttribute, nReadErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadTimeAttribute
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::ReadTimeAttribute( const std::string& strAttribute, T& value, ErrorPolicyType nErrorPolicy )
{
    std::string strTmp;
    if ( archive_.ReadAttribute( strAttribute, strTmp ) && TransformTimeValue( strTmp, value ) )
    {
        std::stringstream strFormatAttr;
        strFormatAttr << strAttribute << "=\"" << value << '\"';
        contexts_.back().lAttributes_.push_back( strFormatAttr.str() );
        return true;
    }
    ProcessError( std::string( "read attribute " ) += strAttribute, nErrorPolicy );
    return false;
}



// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadTime
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T, typename checker>
bool MT_InputArchive_Logger<Archive>::ReadTime( T& value, const checker& checker, ErrorPolicyType nCheckErrorPolicy, ErrorPolicyType nReadErrorPolicy )
{
    std::string strTmp;
    if ( archive_.Read( strTmp ) && TransformTimeValue( strTmp, value ) )
    {
        if ( !checker( value ) )
        {            
            ProcessCheckError( value, checker.Message(), nCheckErrorPolicy );
            return false;
        }
        return true;
    }
    ProcessError( std::string( "read section value " ), nReadErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::ReadTime
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template <typename Archive>
template <typename T>
bool MT_InputArchive_Logger<Archive>::ReadTime( T& value, ErrorPolicyType nErrorPolicy )
{
    std::string strTmp;
    if ( archive_.Read( strTmp ) && TransformTimeValue( strTmp, value ) )
        return true;
    ProcessError( std::string( "read section value " ), nErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::BeginList
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
bool MT_InputArchive_Logger<Archive>::BeginList( const std::string& strList, ErrorPolicyType nErrorPolicy )
{
    if ( archive_.BeginList( strList ) )
    {
        contexts_.push_back( sContext() );
        contexts_.back().sName_ = strList;
        return true;
    }
    ProcessError( std::string( "open list " ) += strList, nErrorPolicy );
    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::NextListElement
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
bool MT_InputArchive_Logger<Archive>::NextListElement()
{
    return archive_.NextListElement();
}


// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::EndList
// Created: JVT 2004-08-23
// -----------------------------------------------------------------------------
template< typename Archive >
inline
bool MT_InputArchive_Logger<Archive>::EndList( ErrorPolicyType nErrorPolicy )
{
    if( archive_.EndList() )
    {
        contexts_.pop_back();
        return true;
    }
    ProcessError( std::string( "close list " ) += contexts_.back().sName_, nErrorPolicy );
    return false;
}

// -----------------------------------------------------------------------------
// Name: MT_InputArchive_Logger::GetCurrentElementName
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
template< typename Archive >
inline
std::string MT_InputArchive_Logger<Archive>::GetCurrentElementName()
{
    return archive_.GetCurrentElementName();
}
