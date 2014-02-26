// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __GhostCommandPostAttributes_h_
#define __GhostCommandPostAttributes_h_

#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

// =============================================================================
/** @class  GhostCommandPostAttributes
    @brief  GhostCommandPostAttributes
*/
// Created: ABR 2012-07-09
// =============================================================================
class GhostCommandPostAttributes : public kernel::CommandPostAttributes_ABC
                                 , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostCommandPostAttributes( const kernel::Entity_ABC& entity, bool isCommandPost, kernel::Controller& controller, gui::PropertiesDictionary& dictionary );
             GhostCommandPostAttributes( const kernel::Entity_ABC& entity, xml::xistream& xis, kernel::Controller& controller, gui::PropertiesDictionary& dictionary );
    virtual ~GhostCommandPostAttributes();
    //@}

    //! @name CommandPostAttributes_ABC operations
    //@{
    virtual bool IsCommandPost() const;
    //@}

    //! @name Serializable_ABC operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Update( const bool& commandPost );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    kernel::Controller&       controller_;
    bool                      isPc_;
    //@}
};

#endif // __GhostCommandPostAttributes_h_
