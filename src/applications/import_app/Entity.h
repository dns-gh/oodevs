// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Entity_h_
#define __Entity_h_

#include "Mapping.h"
#include "Position.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <xeumeuleu/xml.h>

// =============================================================================
/** @class  Entity
    @brief  Entity
*/
// Created: LDC 2010-07-08
// =============================================================================
class Entity
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity();
    virtual ~Entity();
    //@}

    //! @name Operations
    //@{
    static Entity* Find( const std::string& id );
    void Write( xml::xostream& xos, Mapping& mapping ) const;
    std::string Read( xml::xistream& xis, Mapping& mapping, unsigned int knowledgeGroup );
    void SetPosition( const Position& position );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity( const Entity& );            //!< Copy constructor
    Entity& operator=( const Entity& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    bool IsAutomat( const Mapping& mapping ) const;
    bool IsUnit( const Mapping& mapping ) const;
    const Entity* HasUnitChild( const Mapping& mapping ) const;
    void ReadChild( xml::xistream& xis, Mapping& mapping, unsigned int knowledgeGroup );
    void WrapAutomat( xml::xostream& xos, Mapping& mapping, const Entity& unit ) const;
    void WriteAutomat( xml::xostream& xos, Mapping& mapping ) const;
    void WriteAutomat( xml::xostream& xos, Mapping& mapping, const std::string& type ) const;
    void WriteFormation( xml::xostream& xos, Mapping& mapping ) const;
    void WriteUnit( xml::xostream& xos, const Mapping& mapping ) const;
    //@}

private:
    typedef std::vector< boost::shared_ptr< Entity > > T_Entities;
    //! @name Member data
    //@{
    unsigned int id_;
    unsigned int knowledgeGroupId_;
    std::string name_;
    std::string type_;
    T_Entities children_;
    Position position_;
    static std::map< std::string, Entity* > map_;
    //@}
};

#endif // __Entity_h_
