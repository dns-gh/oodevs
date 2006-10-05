// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-09 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Pawn_Type_h_
#define __Mission_Pawn_Type_h_

#include "Types.h"

namespace TEST {

    class Pawn;
    class Mission_Pawn_ABC;

// =============================================================================
/** @class  Mission_Pawn_Type
    @brief  Mission_Pawn_Type
    @par    Using example
    @code
    Mission_Pawn_Type;
    @endcode
*/
// Created: SBO 2005-08-09
// =============================================================================
class Mission_Pawn_Type
{

public:
    //! @name Operations
    //@{
    static void Initialize();
    static void Terminate ();
    static const Mission_Pawn_Type* Find( const std::string& strName );

    template< typename Mission >
    static void Register( const std::string& strName );
    static Mission_Pawn_ABC& CreateMission( const std::string&       strName, Pawn& target );
    static Mission_Pawn_ABC& CreateMission( const Mission_Pawn_Type& type   , Pawn& target );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Mission_Pawn_Type* > T_MissionPawnTypeMap;
    typedef T_MissionPawnTypeMap::const_iterator        CIT_MissionPawnTypeMap;

    typedef Mission_Pawn_ABC&   (* T_MissionAllocator )( Pawn& target );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             Mission_Pawn_Type( const std::string& strName, T_MissionAllocator missionAllocator );
    virtual ~Mission_Pawn_Type();
    //@}

    //! @name Copy/Assignment
    //@{
    Mission_Pawn_Type( const Mission_Pawn_Type& );            //!< Copy constructor
    Mission_Pawn_Type& operator=( const Mission_Pawn_Type& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    Mission_Pawn_ABC& Instanciate( Pawn& target ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string        strName_;
    const T_MissionAllocator missionAllocator_;
    //@}

    //! @name Static members
    //@{
    static T_MissionPawnTypeMap     missions_;
    //@}
};

} // end namespace TEST

#include "Mission_Pawn_Type.inl"

#endif // __Mission_Pawn_Type_h_
