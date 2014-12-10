// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 MASA Group
//
// *****************************************************************************

#ifndef __NoSideHelpers_h_
#define __NoSideHelpers_h_

#include "EntityImplementation.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"

namespace gui
{

    class NoSideDiplomacy : public kernel::Diplomacies_ABC
    {
    public:
        NoSideDiplomacy() {}
        ~NoSideDiplomacy() {}

    public:
        virtual const kernel::Karma& GetDiplomacy( const kernel::Entity_ABC& ) const
        {
            return kernel::Karma::unknown_; // neutral?
        }
        virtual const kernel::Karma& GetKarma() const
        {
            return kernel::Karma::unknown_;
        }
    };

    class NoSideTeam : public gui::EntityImplementation< kernel::Team_ABC >
    {
    public:
        NoSideTeam( kernel::Controller& controller,
                    unsigned long id,
                    const QString& name,
                    actions::ActionsModel* actionsModel = 0 )
            : gui::EntityImplementation< kernel::Team_ABC >( controller, id, name, actionsModel )
        {
                        // NOTHING
        }
        virtual ~NoSideTeam() {}

        virtual bool CanBeRenamed() const
        {
            return false;
        }
    };

} //! namespace gui

#endif // __NoSideHelpers_h_
