// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ExcludeRegExpValidator_h_
#define __ADN_ExcludeRegExpValidator_h_

class ADN_ExcludeRegExpValidator : public QRegExpValidator
{
public:
    ADN_ExcludeRegExpValidator( const QString& regExp, QObject* parent = 0 )
        : QRegExpValidator( QRegExp( regExp ), parent )
    {}
    ADN_ExcludeRegExpValidator( const QRegExp& regExp, QObject* parent = 0 )
        : QRegExpValidator( regExp, parent )
    {}

    virtual void fixup( QString& input ) const
    {
        int index = regExp().lastIndexIn( input );
        if( index != -1 )
            input.remove( index, 1 );
    }
    virtual State validate( QString& input, int& ) const
    {
        return regExp().lastIndexIn( input ) == -1 ? Acceptable : Invalid;
    }
};

#endif // __ADN_ExcludeRegExpValidator_h_
