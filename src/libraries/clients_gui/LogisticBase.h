// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogisticBase_h_
#define __LogisticBase_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controller;
    class Controllers;
    class Entity_ABC;
}

namespace sword
{
    enum EnumLogisticLevel;
}

namespace gui
{
    class PropertiesDictionary;

// =============================================================================
/** @class  LogisticBase
    @brief  LogisticBase
*/
// Created: ABR 2014-01-17
// =============================================================================
class LogisticBase : public kernel::Extension_ABC
                   , public kernel::Serializable_ABC
                   , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticBase( kernel::Controllers& controllers,
                           const kernel::Entity_ABC& entity,
                           gui::PropertiesDictionary& dictionary,
                           bool active,
                           bool isBase );
             LogisticBase( kernel::Controllers& controllers,
                           const kernel::Entity_ABC& entity,
                           gui::PropertiesDictionary& dictionary,
                           bool active,
                           xml::xistream& xis );
    virtual ~LogisticBase();
    //@}

    //! @name accessors
    //@{
    bool IsBase() const;
    void SetBase( const bool& isBase );
    const kernel::Entity_ABC& GetEntity() const;
    //@}

private:
    //! @name Serializable_ABC implementation
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary,
                           bool active,
                           bool readOnly );
    //@}

private:
    //! @name Attributes
    //@{
    kernel::Controller& controller_;
    bool isBase_;
    const kernel::Entity_ABC& entity_;
    //@}
};

} //! namespace gui

#endif // __LogisticBase_h_
