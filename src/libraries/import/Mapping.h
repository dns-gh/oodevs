// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef import_Mapping_h
#define import_Mapping_h

#include <xeumeuleu/xml.hpp>
#include <string>
#include <map>
#include <set>

class Warnings;

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
             Mapping( const std::string& file, Warnings& warnings );
    virtual ~Mapping();
    //@}

    //! @name Operations
    //@{
    unsigned int AddId( const std::string& editorId );
    unsigned int AddId();
    //@}

    //! @name Accessors
    //@{
    bool IsUnit( const std::string& type ) const;
    bool IsAutomat( const std::string& type ) const;
    bool IsCommandPost( const std::string& type ) const;

    std::string GetSuperiorId( const std::string& type ) const;
    std::string GetMissionParameterName( int index ) const;
    std::string GetMissionParameterType( int index ) const;
    std::string GetMissionParameterLocationType( int index ) const;
    //@}

    //! @name Operators
    //@{
    std::string operator[]( const std::string& key ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Mapping( const Mapping& );            //!< Copy constructor
    Mapping& operator=( const Mapping& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAssociation( xml::xistream& xis );
    void ReadAutomat( xml::xistream& xis );
    void ReadUnit( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    Warnings& warnings_;
    std::map< std::string, std::string > mapping_;
    std::set< std::string > automats_;
    std::set< std::string > commandPosts_;
    std::map< std::string, std::string > units_;
    unsigned int maxId_;
    //@}
};

#endif // import_Mapping_h
