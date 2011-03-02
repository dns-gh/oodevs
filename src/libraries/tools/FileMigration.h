// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef FileMigration_h
#define FileMigration_h

#include "FileMigration_ABC.h"
#include <map>

namespace tools
{
// =============================================================================
/** @class  FileMigration
    @brief  FileMigration
*/
// Created: NLD 2011-02-14
// =============================================================================
class FileMigration : public FileMigration_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FileMigration( xml::xistream& xis );
    virtual ~FileMigration();
    //@}
    
    //! @name Accessors
    //@{
    virtual const std::string& GetFromVersion() const { return fromVersion_; }
    virtual const std::string& GetToVersion  () const { return toVersion_; }
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< xml::xistream > UpgradeFile( std::auto_ptr< xml::xistream > file, const std::string& schema ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_XslTransforms;
    //@}

private:
    //! @name Tools
    //@{
    void ReadXslTransform( xml::xistream& xis );
    //@}

private:
    const std::string fromVersion_;
    const std::string toVersion_;
    T_XslTransforms transformsFromSchema_;
    T_XslTransforms transformsFromRootNode_;
};

} // end namespace tools

#endif // FileMigration_h
