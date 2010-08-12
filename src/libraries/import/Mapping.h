// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// LTO
//
// *****************************************************************************

#ifndef import_Mapping_h
#define import_Mapping_h

#include "Position.h"
#include <xeumeuleu/xml.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>
#include <vector>
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
    unsigned int AddId( const std::string& id );
    unsigned int AddId();

    void Serialize( xml::xosubstream xos, const std::string& id, const std::vector< std::vector< Position > >& tacticals ) const;
    //@}

    //! @name Accessors
    //@{
    bool IsUnit( const std::string& type ) const;
    bool IsAutomat( const std::string& type ) const;
    bool IsCommandPost( const std::string& type ) const;

    std::string GetSuperiorId( const std::string& type ) const;
    //@}

    //! @name Operators
    //@{
    std::string operator[]( const std::string& id ) const;
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
    void ReadMission( xml::xistream& xis );
    void ReadParameter( xml::xistream& xis, const std::string& key );

    template< int >
    std::string GetParameter( const std::string& id, std::size_t index ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::vector< boost::tuple< std::string, std::string, std::string > > > T_Missions;
    typedef T_Missions::const_iterator                                                                  CIT_Missions;
    //@}

private:
    //! @name Member data
    //@{
    Warnings& warnings_;
    std::map< std::string, std::string > mapping_;
    std::set< std::string > automats_;
    std::set< std::string > commandPosts_;
    std::map< std::string, std::string > units_;
    T_Missions missions_;
    unsigned int maxId_;
    //@}
};

#endif // import_Mapping_h
