// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_SERIALIZERS_H__
#define PROTOCOL_SERIALIZERS_H__

#include "Simulation.h"

#include <boost/noncopyable.hpp>
#include <cstdint>

namespace xml
{
    class xistream;
};

namespace protocol
{
namespace serializer
{
    struct Service_ABC : public boost::noncopyable
    {
        //! @name Constructors/Destructor
        //@{
                 Service_ABC() {}
        virtual ~Service_ABC() {}
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
            AGENT,
            AUTOMAT,
            INHABITANT,
            FORMATION,
            KNOWLEDGE_GROUP,
            OBJECT,
            POPULATION,
            TEAM,
            URBAN_OBJECT,
            COUNT,
        };
        virtual EntityType Resolve( uint32_t id ) const = 0;
        //@}

    };

    class Reader : public boost::noncopyable
    {
    public:
        //! @name Constructors/Destructor
        //@{
                 Reader( const Service_ABC& converter );
        virtual ~Reader();
        //@}

        //! @name Public methods
        //@{
        void Read( sword::MissionParameter& dst,     xml::xistream& xis ) const;
        void Read( sword::MissionParameters& dst,    xml::xistream& xis ) const;
        void Read( sword::UnitOrder& dst,            xml::xistream& xis ) const;
        void Read( sword::AutomatOrder& dst,         xml::xistream& xis ) const;
        void Read( sword::CrowdOrder& dst,           xml::xistream& xis ) const;
        void Read( sword::FragOrder& dst,            xml::xistream& xis ) const;
        void Read( sword::MagicAction& dst,          xml::xistream& xis ) const;
        void Read( sword::UnitMagicAction& dst,      xml::xistream& xis ) const;
        void Read( sword::ObjectMagicAction& dst,    xml::xistream& xis ) const;
        void Read( sword::KnowledgeMagicAction& dst, xml::xistream& xis ) const;
        void Read( sword::SetAutomatMode& dst,       xml::xistream& xis ) const;
        //@}

    private:
        //! @name Private members
        //@{
        const Service_ABC& service_;
        //@}
    };
}
}

#endif // PROTOCOL_SERIALIZERS_H__
