// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Mapping_h_
#define __Mapping_h_

#include <map>
#include <set>
#include <string>
#include <xeumeuleu/xml.h>

// =============================================================================
/** @class  Mapping
    @brief  Mapping
*/
// Created: LDC 2010-07-07
// =============================================================================
class Mapping
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Mapping( const std::string& file );
    virtual ~Mapping();
    //@}

    //! @name Operations
    //@{
    std::string operator[]( const std::string& key ) const;
    void AddWarning( const std::string& id );
    void LogWarnings( std::ostream& os ) const;
    unsigned int AddId( const std::string& editorId );
    unsigned int AddId();
    bool IsUnit( const std::string& type ) const;
    bool IsAutomat( const std::string& type ) const;
    bool IsCommandPost( const std::string& type ) const;
    std::string GetSuperiorId( const std::string& type ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Mapping( const Mapping& );            //!< Copy constructor
    Mapping& operator=( const Mapping& ); //!< Assignment operator
    //@}

    void WriteAssociation( xml::xistream& xis );
    void WriteAutomat( xml::xistream& xis );
    void WriteUnit( xml::xistream& xis );

private:
    //! @name Member data
    //@{
    std::map< std::string, std::string > mapping_;
    std::set< std::string > automats_;
    std::set< std::string > commandPosts_;
    std::map< std::string, std::string > units_;
    mutable std::set< std::string > unknownKeys_;
    unsigned int maxId_;
    //@}
};

#endif // __Mapping_h_
