// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_Logger_ABC_h_
#define __MT_Logger_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

//=============================================================================
/**
    @class  MT_Logger_ABC
*/
// Created:  NLD 00-06-05
//=============================================================================
class MT_Logger_ABC : private boost::noncopyable
{
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    enum E_LogLevel
    {
        eLogLevel_FatalError = 0x01, // Program can't continue : abort immediately
        eLogLevel_Error      = 0x02, // Serious error, user must be informed about it
        eLogLevel_Warning    = 0x04, // User is normally informed about it but may be ignored
        eLogLevel_Message    = 0x08, // Normal message (i.e. normal output of a non GUI app)
        eLogLevel_Info       = 0x10, // Informational messages
        eLogLevel_Verbose    = 0x20, // Messages only enabled in verbose mode
        eLogLevel_Debug      = 0x40, // Messages only enabled in debug mode
        eLogLevel_None       = 0x60,
        eLogLevel_All        = 0xFF
    };
    //@}

public:
    explicit MT_Logger_ABC( int nLogLevels = eLogLevel_All );
    virtual ~MT_Logger_ABC();

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    void Log   ( E_LogLevel nLevel, const char* strMessage, const char* strContext = 0, int nCode = -1 );
    void Pause (); //!< Pause the logger : the message aren't logged anymore.
    void Resume(); //!< Resume the logger.
    //@}

    //-------------------------------------------------------------------------
    /** @name Timestamp */
    //-------------------------------------------------------------------------
    //@{
    void        SetTimestampFormat( const char* strFormat ); //!< Set the timestamp format. See strftime() man for formats details.
    const char* GetTimestampFormat() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Log levels */
    //-------------------------------------------------------------------------
    //@{
    bool IsLogLevelSet( E_LogLevel nLevel ) const;
    //@}

protected:
    //-------------------------------------------------------------------------
    /** @name Log methods */
    //-------------------------------------------------------------------------
    //@{
    virtual void LogString( E_LogLevel nLevel, const char* szMsg, const char* strContext, int nCode ) = 0;
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    const char* GetTimestampAsString();
    const char* GetLogLevelAsString( E_LogLevel nLevel);
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Member data */
    //-------------------------------------------------------------------------
    //@{
    bool        bPaused_;
    std::string strTimestamp_;
    int         nLogLevels_;
    //@}
};

#endif // __MT_Logger_ABC_h_
