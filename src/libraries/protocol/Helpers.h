// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef PROTOCOL_HELPERS_H__
#define PROTOCOL_HELPERS_H__

#include "Dispatcher.h"
#include "Simulation.h"

namespace protocol
{
namespace mapping
{
    struct MagicUnitAction
    {
        typedef sword::UnitMagicAction::Type value_type;
        std::string                  name;
        value_type                   type;
        static const MagicUnitAction data_[];
        static const size_t          size_;
    };

    struct MagicKnowledgeAction
    {
        typedef sword::KnowledgeMagicAction::Type value_type;
        std::string                       name;
        value_type                        type;
        static const MagicKnowledgeAction data_[];
        static const size_t               size_;
    };

    struct MagicObjectAction
    {
        typedef sword::ObjectMagicAction::Type value_type;
        std::string                    name;
        value_type                     type;
        static const MagicObjectAction data_[];
        static const size_t            size_;
    };

    struct MagicAction
    {
        typedef sword::MagicAction::Type value_type;
        std::string              name;
        sword::MagicAction::Type type;
        static const MagicAction data_[];
        static const size_t      size_;
    };

    struct PhaseLineType
    {
        typedef sword::PhaseLineOrder::Function value_type;
        std::string                name;
        value_type                 type;
        static const PhaseLineType data_[];
        static const size_t        size_;
    };

    struct Service
    {
        typedef sword::Service value_type;
        std::string          name;
        value_type           type;
        static const Service data_[];
        static const size_t  size_;
    };
}
}

#endif // PROTOCOL_HELPERS_H__
