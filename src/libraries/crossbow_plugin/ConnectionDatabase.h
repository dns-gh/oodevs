// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConnectionDatabase_h_
#define __ConnectionDatabase_h_

#include "Database.h"

namespace plugins
{
namespace crossbow
{
    class Table_ABC;

// =============================================================================
/** @class  ConnectionDatabase
    @brief  ConnectionDatabase
*/
// Created: JCR 2009-02-03
// =============================================================================
class ConnectionDatabase : public Database
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ConnectionDatabase( const std::string& url );
    virtual ~ConnectionDatabase();
    //@}

    //! @name 
    //@{
    virtual Table_ABC&  OpenBufferedTable( const std::string& name, bool clear = true );
    virtual Table_ABC*  OpenTable( const std::string& name );
    virtual void        ClearTable( const std::string& name );
    virtual void ReleaseTable( const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConnectionDatabase( const ConnectionDatabase& );            //!< Copy constructor
    ConnectionDatabase& operator=( const ConnectionDatabase& ); //!< Assignment operator
    //@}

    //! @name 
    //@{
    std::string Decorate( const std::string& name );
    IPropertySetPtr CreateProperty( const std::string& url );
    //@}

private:
    //! @name Attributes
    //@{
    std::string database_;
    std::string protocol_;
    //@}
};

}
}

#endif // __ConnectionDatabase_h_
