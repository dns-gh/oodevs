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

#ifndef __Mission_Population_Type_h_
#define __Mission_Population_Type_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

namespace TEST {

    class Population;
    class Mission_Population_ABC;

// =============================================================================
// Created: SBO 2005-08-09
// =============================================================================
class Mission_Population_Type
{

public:
    //! @name Operations
    //@{
    static void Initialize();
    static void Terminate ();
    static const Mission_Population_Type* Find( const std::string& strName );

    template< typename Mission >
    static void Register( const std::string& strName );
    static Mission_Population_ABC& CreateMission( const std::string&             strName, Population& target );
    static Mission_Population_ABC& CreateMission( const Mission_Population_Type& type   , Population& target );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Mission_Population_Type* > T_MissionPopulationTypeMap;
    typedef T_MissionPopulationTypeMap::const_iterator        CIT_MissionPopulationTypeMap;

    typedef Mission_Population_ABC&   (* T_MissionAllocator )( Population& target );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             Mission_Population_Type( const std::string& strName, T_MissionAllocator missionAllocator );
    virtual ~Mission_Population_Type();
    //@}

    //! @name Copy/Assignment
    //@{
    Mission_Population_Type( const Mission_Population_Type& );            //!< Copy constructor
    Mission_Population_Type& operator=( const Mission_Population_Type& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    Mission_Population_ABC& Instanciate( Population& target ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string        strName_;
    const T_MissionAllocator missionAllocator_;
    //@}

    //! @name Static members
    //@{
    static T_MissionPopulationTypeMap     missions_;
    //@}
};

} // end namespace TEST

#include "Mission_Population_Type.inl"

#endif // __Mission_Population_Type_h_
