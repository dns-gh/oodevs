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

#ifndef __PawnModel_h_
#define __PawnModel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "Actions/Missions/Mission_Pawn_Type.h"

namespace TEST {

// =============================================================================
/** @class  PawnModel
    @brief  PawnModel
    @par    Using example
    @code
    PawnModel;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class PawnModel
{

public:
    //! @name Static methods
    //@{
    static       void       Initialize( const std::string& strScipioModelFile );
    static       void       Terminate ();
    static const PawnModel* Find      ( const std::string& strModel );
    //@}
    
    //! @name Accessors
    //@{
    const T_MissionPawnTypeVector& GetMissions() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             PawnModel( const std::string& strName, XmlInputArchive& archive );
    virtual ~PawnModel();
    //@}

    //! @name Copy/Assignment
    //@{
    PawnModel( const PawnModel& );            //!< Copy constructor
    PawnModel& operator=( const PawnModel& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, PawnModel* >     T_PawnModelMap;
    typedef T_PawnModelMap::const_iterator          CIT_PawnModelMap;
    //@}

private:
    //! @name Member data
    //@{
    std::string             strName_;
    T_MissionPawnTypeVector missionTypes_;

    // global type list
    static T_PawnModelMap   models_;
    //@}
};

} // end namespace TEST

#include "PawnModel.inl"

#endif // __PawnModel_h_
