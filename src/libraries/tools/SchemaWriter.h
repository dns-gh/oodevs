// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SchemaWriter_h__
#define __SchemaWriter_h__

#include "SchemaWriter_ABC.h"

namespace tools
{

// =============================================================================
/** @class  SchemaWriter
    @brief  Schema writer definition
*/
// Created: NLD 2011-02-23
// =============================================================================
class SchemaWriter : public SchemaWriter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             SchemaWriter();
    virtual ~SchemaWriter();
    //@}

    //! @name Operations
    //@{
    virtual void WriteSchema( xml::xostream& xos, const Path& category, const Path& schemaName ) const;
    virtual void WritePhysicalSchema( xml::xostream& xos, const Path& schemaName ) const;
    virtual void WriteExerciseSchema( xml::xostream& xos, const Path& schemaName ) const;
    //@}
};

}

#endif // __SchemaWriter_h__
