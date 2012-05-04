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

#include "clients_kernel/CommandPostAttributes.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class PropertiesDictionary;
    class AgentType;
    class Controller;
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
class CommandPostAttributes : public kernel::CommandPostAttributes
                            , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CommandPostAttributes( xml::xistream& xis, kernel::Controller& controller, const kernel::Entity_ABC& entity, const kernel::AgentType& type,
                                    kernel::PropertiesDictionary& dictionary );
             CommandPostAttributes( const kernel::Entity_ABC& entity, kernel::Controller& controller, const kernel::AgentType& type,
                                    kernel::PropertiesDictionary& dictionary, bool commandPost = false );
    virtual ~CommandPostAttributes();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary );
    void Update( const bool& commandPost );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    //@}
};

#endif // __CommandPostAttributes_h_
