// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_XML_WRITERS_H__
#define PROTOCOL_XML_WRITERS_H__

#include <boost/noncopyable.hpp>
#include <cstdint>
#include <string>

namespace xml
{
    class xostream;
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
    struct Writer_ABC : public boost::noncopyable
    {
        //! @name Constructors/Destructor
        //@{
                 Writer_ABC() {}
        virtual ~Writer_ABC() {}
        //@}

        //! @name Point conversions
        //@{
        // convert from wgs84 to mgrs
        virtual std::string Convert( double x, double y ) const = 0;
        //@}
    };

    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::MissionParameter& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::MissionParameters& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::UnitOrder& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::AutomatOrder& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::CrowdOrder& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::FragOrder& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::MagicAction& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::UnitMagicAction& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::ObjectMagicAction& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::KnowledgeMagicAction& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::SetAutomatMode& src );
    void Write( xml::xostream& xos, const Writer_ABC& writer, const sword::ClientToSim_Content& src );
}

#endif // PROTOCOL_XML_WRITERS_H__
