// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CommandPostAttributes_h_
#define __CommandPostAttributes_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class AgentType;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  CommandPostAttributes
    @brief  CommandPostAttributes
*/
// Created: SBO 2007-03-27
// =============================================================================
class CommandPostAttributes : public kernel::CommandPostAttributes_ABC
                            , public kernel::Serializable_ABC
                            , public gui::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandPostAttributes( xml::xistream& xis, const kernel::Entity_ABC& entity, const kernel::AgentType& type,
                                    gui::PropertiesDictionary& dictionary );
             CommandPostAttributes( const kernel::Entity_ABC& entity, const kernel::AgentType& type,
                                    gui::PropertiesDictionary& dictionary, bool commandPost = false );
    virtual ~CommandPostAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsCommandPost() const;
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
    const kernel::AgentType& type_;
    bool commandPost_;
    //@}
};

#endif // __CommandPostAttributes_h_
