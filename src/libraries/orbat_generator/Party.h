// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#ifndef orbat_generator_Party_h
#define orbat_generator_Party_h

#include "Formation.h"
#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;

namespace xml
{
    class xistream;
    class xostream;
}

namespace orbat_generator
{

enum PartySide
{
    Friend = 0,
    Enemy,
    Neutral
};

// =============================================================================
/** @class  Party
*/
// Created: RCD 2011-03-02
// =============================================================================

class Party : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Party( const PartySide& side );
    virtual ~Party();
    //@}

public:
    //! @name Operations
    //@{
    void GenerateDiplomacy( xml::xostream& orbat, const unsigned int id, const std::string& side );
    void GenerateDiplomacy( xml::xostream& orbat, const unsigned int id, std::vector< boost::shared_ptr< Party > > parties );
    void InsertIntoOrbat( xml::xostream& orbat, const IdNameGenerator& idGen );
    void AddFormation( boost::shared_ptr< Formation > formation );
    void AddAutomate( boost::shared_ptr< Automate > automate );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< Formation > > formations_;
    const std::string side_;
    unsigned int current_;
    //@}
};

}

#endif // orbat_generator_Facade_h
