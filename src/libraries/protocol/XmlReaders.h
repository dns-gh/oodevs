// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_XML_READERS_H__
#define PROTOCOL_XML_READERS_H__

#include <boost/noncopyable.hpp>
#include <cstdint>
#include <string>

namespace xml
{
    class xistream;
}

namespace sword
{
    class AutomatOrder;
    class ClientToSim_Content;
    class CrowdOrder;
    class FragOrder;
    class KnowledgeMagicAction;
    class MagicAction;
    class MissionParameter;
    class MissionParameters;
    class ObjectMagicAction;
    class SetAutomatMode;
    class UnitMagicAction;
    class UnitOrder;
}

namespace protocol
{
    struct Reader_ABC : public boost::noncopyable
    {
        //! @name Constructors/Destructor
        //@{
                 Reader_ABC() {}
        virtual ~Reader_ABC() {}
        //@}

        //! @name Point conversions
        //@{
        struct  Point { double x, y; };
        // convert from mgrs to wgs84
        virtual Point Convert( const std::string& value ) const = 0;
        //@}

        //! @name Entity identifier
        //@{
        enum EntityType
        {
            AUTOMAT,
            CROWD,
            FORMATION,
            PARTY,
            POPULATION,
            UNIT,
            COUNT,
        };
        virtual EntityType Resolve( uint32_t id ) const = 0;
        //@}
    };

    void Read( const Reader_ABC& reader, sword::MissionParameter& dst,     xml::xistream& xis, bool firstLevelParam = false );
    void Read( const Reader_ABC& reader, sword::MissionParameters& dst,    xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::UnitOrder& dst,            xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::AutomatOrder& dst,         xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::CrowdOrder& dst,           xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::FragOrder& dst,            xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::MagicAction& dst,          xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::UnitMagicAction& dst,      xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::ObjectMagicAction& dst,    xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::KnowledgeMagicAction& dst, xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::SetAutomatMode& dst,       xml::xistream& xis );
    void Read( const Reader_ABC& reader, sword::ClientToSim_Content& dst,  xml::xistream& xis );
}

#endif // PROTOCOL_XML_READERS_H__
