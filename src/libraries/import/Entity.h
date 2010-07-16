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
#include <xeumeuleu/xml.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

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
             Entity( xml::xistream& xis, Mapping& mapping, unsigned int knowledgeGroup );
    virtual ~Entity();
    //@}

    //! @name Operations
    //@{
    static Entity* Find( const std::string& id );
    //@}

    //! @name Modifiers
    //@{
    void SetPosition( const Position& position );
    //@}

    //! @name Operators
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Entity& entity );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity( const Entity& );            //!< Copy constructor
    Entity& operator=( const Entity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Write( xml::xostream& xos ) const;
    bool IsAutomat() const;
    bool IsUnit() const;
    const Entity* HasUnitChild() const;
    void ReadChild( xml::xistream& xis, unsigned int knowledgeGroup );
    void WrapAutomat( xml::xostream& xos, const Entity& unit ) const;
    void WriteAutomat( xml::xostream& xos, const std::string& type ) const;
    void WriteFormation( xml::xostream& xos ) const;
    void WriteUnit( xml::xostream& xos ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< Entity > > T_Entities;
    //@}

private:
    //! @name Member data
    //@{
    Mapping& mapping_;
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
