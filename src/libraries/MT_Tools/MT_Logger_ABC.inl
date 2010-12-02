// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::SetTimestampFormat
/** @param  strFormat Format of the timestamp

  Set the timestamp format. See strftime() man for formats details.
*/
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::SetTimestampFormat( const char* strFormat )
{
    strTimestamp_ = strFormat;
}


//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::GetTimestampFormat
/**
    @return PUT YOUR RETURN VALUE AND ITS EXPLANATION

  PUT YOUR COMMENTS HERE
*/
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
const char* MT_Logger_ABC::GetTimestampFormat() const
{
    return strTimestamp_.c_str();
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::SetLogLevels
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::SetLogLevels( E_LogLevel nLevels )
{
    nLogLevels_ = nLevels;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::AddLogLevel
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::AddLogLevel( E_LogLevel nLevel )
{
    // $$$$ CBX 2003-02-13: &= --> |= for boolean addition
//    nLogLevels_ &= nLevel;
    nLogLevels_ |= nLevel;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::RemoveLogLevel
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::RemoveLogLevel( E_LogLevel nLevel )
{
    nLogLevels_ &= ~nLevel;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::IsLogLevelSet
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
bool MT_Logger_ABC::IsLogLevelSet( E_LogLevel nLevel ) const
{
    return( (nLogLevels_ & nLevel) != 0 );
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::SetLogLayers
// Created:  NLD 2001-08-20 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::SetLogLayers( unsigned int nLayers )
{
    nLogLayers_ = nLayers;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::AddLogLayer
// Created:  NLD 2001-08-20 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::AddLogLayer( unsigned int nLayer )
{
    // $$$$ CBX 2003-02-13: &= --> |= for boolean addition
//    nLogLayers_ &= nLayer;
    nLogLayers_ |= nLayer;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::RemoveLogLayer
// Created:  NLD 2001-08-20 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::RemoveLogLayer( unsigned int nLayer )
{
    nLogLayers_ &= ~nLayer;
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::IsLogLayerSet
// Created:  NLD 2001-08-20 
//-----------------------------------------------------------------------------
inline
bool MT_Logger_ABC::IsLogLayerSet( unsigned int nLayer ) const
{
    return( nLayer == 0 || nLogLayers_ & nLayer );
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::Pause
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::Pause()
{
    bPaused_ = true;  
}

//-----------------------------------------------------------------------------
// Name: MT_Logger_ABC::Resume
// Created:  NLD 00-06-05 
//-----------------------------------------------------------------------------
inline
void MT_Logger_ABC::Resume()
{
    bPaused_ = false;
}
