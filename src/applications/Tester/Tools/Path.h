// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Path.h $
// $Author: Sbo $
// $Modtime: 19/07/05 10:50 $
// $Revision: 2 $
// $Workfile: Path.h $
//
// *****************************************************************************

#ifndef __Path_h_
#define __Path_h_

#include "Types.h"

#include "Tools/Position.h"

namespace TEST
{
    
// =============================================================================
/** @class  Path
    @brief  Path
    @par    Using example
    @code
    Path;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class Path
{
public:
    //! @name Types
    //@{
    typedef std::vector< Position* >                 T_PositionVector;
    typedef T_PositionVector::const_iterator         CIT_PositionVector;
    typedef T_PositionVector::const_reverse_iterator CRIT_PositionVector;
    //@}

public:
	//! @name Constructors/Destructor
    //@{
             Path();
    virtual ~Path();
    //@}

    //! @name Accessors
    //@{
    const T_PositionVector& GetPath() const;
    //@}

    //! @name Operations
    //@{
    void AddNode( Position& position );
    void Clear  ();
    //@}

    //! @name Operators
    //@{
    Path& operator=( const Path& path );
    //@}

private:
	//! @name Global area list
    //@{
    T_PositionVector path_;
	//@}
};

} // end namespace TEST

#include "Path.inl"

#endif // __Path_h_