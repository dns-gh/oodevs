// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LogTable_h_
#define __LogTable_h_

namespace geostore
{

// =============================================================================
/** @class  LogTable
    @brief  LogTable
*/
// Created: AME 2010-07-28
// =============================================================================
class LogTable : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogTable( sqlite3* db );
    virtual ~LogTable();
    //@}

    //! @name Operations
    //@{
    void UpdateStatus( const std::string& status );
    //@}

private:
    //! @name Helpers
    //@{
    void GetStatus();
    void CreateStructure();
    //@}

private:
    //! @name Member data
    //@{
    sqlite3* db_;
    std::string name_;
    char* err_msg;
    //@}

public:
    //! @name Member data
    //@{
    bool status_;
    //@}
    
};

} //! namespace geostore

#endif // __LogTable_h_
