// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ProjectionTable_h_
#define __ProjectionTable_h_

namespace geostore
{

// =============================================================================
/** @class  ProjectionTable
    @brief  ProjectionTable
*/
// Created: AME 2010-08-02
// =============================================================================
class ProjectionTable : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ProjectionTable( sqlite3* db );
    virtual ~ProjectionTable();
    //@}
};

} //! namespace geostore

#endif // __ProjectionTable_h_
