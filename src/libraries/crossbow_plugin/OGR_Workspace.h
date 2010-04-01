// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_OGR_Workspace_h_
#define __crossbow_OGR_Workspace_h_

#include "Workspace_ABC.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class Config;
}

namespace plugins
{
namespace crossbow
{

// =============================================================================
/** @class  OGR_Workspace
    @brief  OGR_Workspace
*/
// Created: JCR 2010-02-24
// =============================================================================
class OGR_Workspace : public Workspace_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             OGR_Workspace();
             OGR_Workspace( int argc, char* argv[] );
    virtual ~OGR_Workspace();
    //@}

    //! @name 
    //@{
    void Initialize( xml::xistream& xis, const dispatcher::Config& config );
    void InitializeProperty( xml::xistream& xis, const dispatcher::Config& config );
    void InitializeConnectionReference( const std::string& name, const dispatcher::Config& config, const std::string& reference );
    //@}

    //! @name 
    //@{
    Database_ABC& GetDatabase( const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OGR_Workspace( const OGR_Workspace& );            //!< Copy constructor
    OGR_Workspace& operator=( const OGR_Workspace& ); //!< Assignment operator
    //@}

private:
    typedef boost::shared_ptr< Database_ABC >       T_Database;
    typedef std::map< std::string, T_Database >     T_DatabasesMap;
    typedef std::map< std::string, std::string >    T_DatabasesReferenceMap;
    typedef T_DatabasesReferenceMap::const_iterator CIT_DatabasesReferenceMap;

private:
    //! @name 
    //@{
    T_DatabasesMap          databases_;
    T_DatabasesReferenceMap references_; // use to not open db twice
    //@}
};

}
} // end namespace OGR

#endif // __OGR_Workspace_h_

