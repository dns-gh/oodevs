// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_OGR_Database_h_
#define __crossbow_OGR_Database_h_

#include "Database_ABC.h"
#include <boost/shared_ptr.hpp>

class OGRDataSource;

namespace plugins
{
namespace crossbow
{
// =============================================================================
/** @class  OGR_Database
    @brief  OGR_Database
*/
// Created: JCR 2010-02-24
// =============================================================================
class OGR_Database : public Database_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit OGR_Database( OGRDataSource* source );
             OGR_Database( OGRDataSource* source, const std::string& name, const std::string& schema );
    virtual ~OGR_Database();
    //@}

    //! @name
    //@{
    std::string GetTableName( const std::string& name ) const;
    //@}

    //! @name Operations
    //@{
    Table_ABC&  OpenBufferedTable( const std::string& name, bool clear = true );
    Table_ABC*  OpenTable( const std::string& name );
    void        ClearTable( const std::string& name, const std::string& filter );
    void        Flush();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OGR_Database( const OGR_Database& );            //!< Copy constructor
    OGR_Database& operator=( const OGR_Database& ); //!< Assignment operator
    //@}

private:
    typedef boost::shared_ptr< Table_ABC >      T_TablePtr;
    typedef std::map< std::string, T_TablePtr > T_Tables;
    typedef T_Tables::iterator                  IT_Tables;

private:
    //! @name Member data
    //@{
    T_Tables tables_;
    boost::shared_ptr< OGRDataSource > datasource_;
    const std::string name_;
    const std::string schema_;
    //@}
};

} // end namespace crossbow
} // end namespace plugins

#endif // __OGR_Database_h_
