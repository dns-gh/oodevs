// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SchemaWriter_ABC_h__
#define __SchemaWriter_ABC_h__

#include <boost/noncopyable.hpp>

namespace xml
{
    class xostream;
}

namespace tools
{
    class Path;

// =============================================================================
/** @class  SchemaWriter_ABC
    @brief  Schema writer definition
*/
// Created: NLD 2011-02-23
// =============================================================================
class SchemaWriter_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SchemaWriter_ABC() {}
    virtual ~SchemaWriter_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void WriteSchema( xml::xostream& xos, const Path& category, const Path& schemaName ) const = 0;
    virtual void WritePhysicalSchema( xml::xostream& xos, const Path& schemaName ) const = 0;
    virtual void WriteExerciseSchema( xml::xostream& xos, const Path& schemaName ) const = 0;
    //@}
};

}

#endif // __SchemaWriter_ABC_h__
