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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Tools/Position.h $
// $Author: Sbo $
// $Modtime: 18/07/05 17:42 $
// $Revision: 14 $
// $Workfile: Position.h $
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "Types.h"

namespace TEST
{
    class Position;
    class PositionManager;

//! @name Types
//@{
// TODO : create a line / polygon encapsulation class
typedef std::vector< Position* >                 T_PositionVector;
typedef T_PositionVector::iterator               IT_PositionVector;
typedef T_PositionVector::const_iterator         CIT_PositionVector;
//@}

// =============================================================================
/** @class  Position
    @brief  Position
    @par    Using example
    @code
    Position;
    @endcode
*/
// Created: SBO 2005-05-18
// =============================================================================
class Position
{
public:
	//! @name Constructors/Destructor
    //@{
             Position();
             Position( const std::string& strMgrs );
             Position( double rLatitude, double rLongitude );
    virtual ~Position();
    //@}

    //! @name Static Operations
    //@{
    static void SetPositionManager( PositionManager& positionManager );
    //@}

    //! @name Accessors
    //@{
    std::string GetMgrsCoordinate () const;
    double      GetLatitude       () const;
    double      GetLongitude      () const;
    double      GetSimX           () const;
    double      GetSimY           () const;
    //@}

    //! @name Operations
    //@{
    void   SetSimCoordinates( double rX, double rY );
    //@}

private:
	//! @name Member data
    //@{
    double      rX_;
    double      rY_;

    static PositionManager* pPositionManager_; // TODO: find a way to remove this static
	//@}
};


} // end namespace TEST

#include "Position.inl"

#endif // __Position_h_