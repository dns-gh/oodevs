// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-11 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __Mission_Automat_Type_h_
#define __Mission_Automat_Type_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

namespace TEST {

    class Automat;
    class Mission_Automat_ABC;

// =============================================================================
/** @class  Mission_Automat_Type
    @brief  Mission_Automat_Type
    @par    Using example
    @code
    Mission_Automat_Type;
    @endcode
*/
// Created: SBO 2005-08-09
// =============================================================================
class Mission_Automat_Type
{

public:
    //! @name Operations
    //@{
    static void Initialize();
    static void Terminate ();
    static const Mission_Automat_Type* Find( const std::string& strName );

    template< typename Mission >
    static void Register( const std::string& strName );
    static Mission_Automat_ABC& CreateMission( const std::string& strName, Automat& target, uint nExecutionTick = 0 );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Mission_Automat_Type* > T_MissionAutomatTypeMap;
    typedef T_MissionAutomatTypeMap::const_iterator        CIT_MissionAutomatTypeMap;

    typedef Mission_Automat_ABC&   (* T_MissionAllocator )( Automat& target, uint nExecutionTick );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             Mission_Automat_Type( const std::string& strName, T_MissionAllocator missionAllocator );
    virtual ~Mission_Automat_Type();
    //@}

    //! @name Copy/Assignment
    //@{
    Mission_Automat_Type( const Mission_Automat_Type& );            //!< Copy constructor
    Mission_Automat_Type& operator=( const Mission_Automat_Type& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    Mission_Automat_ABC& Instanciate( Automat& target, uint nExecutionTick = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string              strName_;
    const T_MissionAllocator       missionAllocator_;
    //@}

    //! @name Static members
    //@{
    static T_MissionAutomatTypeMap missions_;
    //@}
};

} // end namespace TEST

#include "Mission_Automat_Type.inl"

#endif // __Mission_Automat_Type_h_
