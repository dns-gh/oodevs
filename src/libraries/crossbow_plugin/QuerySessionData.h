// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_QuerySessionData_h_
#define __crossbow_QuerySessionData_h_

namespace plugins
{
namespace crossbow
{
    class Database_ABC;

// =============================================================================
/** @class  QuerySessionData
    @brief  QuerySessionData
*/
// Created: JCR 2007-04-30
// =============================================================================
class QuerySessionData
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit QuerySessionData( Database_ABC& database );
    virtual ~QuerySessionData();
    //@}

    //! @name Operations
    //@{
    int CreateExercise( const std::string& name );
    int FindExercise( const std::string& name ) const;

    int CreateSession( const std::string& name, int exercise_id );
    int FindSession( const std::string& name, int exercise_id ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    QuerySessionData( const QuerySessionData& );            //!< Copy constructor
    QuerySessionData& operator=( const QuerySessionData& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Database_ABC& database_;
    //@}
};

}
}

#endif // __crossbow_QuerySessionData_h_
