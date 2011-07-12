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
    class Logger_ABC;
}

namespace tools
{
    class ExerciseConfig;
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
             OGR_Workspace( dispatcher::Logger_ABC& logger );
             OGR_Workspace( int argc, char* argv[], dispatcher::Logger_ABC& logger );
    virtual ~OGR_Workspace();
    //@}

    //! @name
    //@{
    void Initialize( xml::xistream& xis, const tools::ExerciseConfig& config );
    void InitializeProperty( const std::string& name, xml::xistream& xis );
    void InitializeConnectionReference( const std::string& name, const std::string& reference );
    //@}

    //! @name
    //@{
    Database_ABC& GetDatabase( const std::string& name );
    void Release( const std::string& name );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OGR_Workspace( const OGR_Workspace& );            //!< Copy constructor
    OGR_Workspace& operator=( const OGR_Workspace& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void InitializeConnection( const std::string& name );
    //@}

private:
    typedef boost::shared_ptr< Database_ABC >       T_Database;
    typedef std::map< std::string, T_Database >     T_DatabasesMap;
    typedef std::map< std::string, std::string >    T_DatabasesReferenceMap;
    typedef std::map< std::string, std::string >    T_DatabasesConnectionMap;
    typedef T_DatabasesReferenceMap::const_iterator CIT_DatabasesReferenceMap;

private:
    //! @name
    //@{
    T_DatabasesMap          databases_;
    T_DatabasesConnectionMap connections_;
    T_DatabasesReferenceMap references_; // use to not open db twice
    std::string             path_;
    dispatcher::Logger_ABC& logger_;
    //@}
};

}
} // end namespace plugins::crossbow

#endif // __OGR_Workspace_h_
