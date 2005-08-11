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

#ifndef __AutomatModel_h_
#define __AutomatModel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

namespace TEST {

    class Mission_Automat_Type;

// =============================================================================
/** @class  AutomatModel
    @brief  AutomatModel
    @par    Using example
    @code
    AutomatModel;
    @endcode
*/
// Created: SBO 2005-08-11
// =============================================================================
class AutomatModel
{

public:
    //! @name Static methods
    //@{
    static       void          Initialize( const std::string& strScipioModelFile );
    static       void          Terminate ();
    static const AutomatModel* Find      ( const std::string& strModel );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             AutomatModel( const std::string& strName, XmlInputArchive& archive );
    virtual ~AutomatModel();
    //@}

    //! @name Copy/Assignment
    //@{
    AutomatModel( const AutomatModel& );            //!< Copy constructor
    AutomatModel& operator=( const AutomatModel& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, AutomatModel* >     T_AutomatModelMap;
    typedef T_AutomatModelMap::const_iterator          CIT_AutomatModelMap;

    typedef std::vector< const Mission_Automat_Type* > T_MissionAutomatTypeVector;
    typedef T_MissionAutomatTypeVector::const_iterator CIT_MissionAutomatTypeVector;
    //@}

private:
    //! @name Member data
    //@{
    std::string                strName_;
    T_MissionAutomatTypeVector missionTypes_;

    // global type list
    static T_AutomatModelMap   models_;
    //@}
};

} // end namespace TEST

#endif // __AutomatModel_h_
