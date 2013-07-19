// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __XQuery_h_
#define __XQuery_h_

namespace tools
{
    class Path;
}

class QString;
class QStringList;

namespace kernel
{
    namespace xquery
    {
        QStringList Evaluate( const tools::Path& xmlFilePath, const QString& queryStr );
        QStringList Evaluate( const QString& text, const QString& queryStr );
    }
}

#endif // __XQuery_h_
