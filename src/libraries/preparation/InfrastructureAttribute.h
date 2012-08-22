// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InfrastructureAttribute_h_
#define __InfrastructureAttribute_h_

#include "Overridable_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/ModesObserver_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Observer_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Displayer_ABC;
    class InfrastructureType;
    class PropertiesDictionary;
    class ObjectTypes;
    class UrbanObject_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  InfrastructureAttribute
    @brief  InfrastructureAttribute
*/
// Created: SLG 2011-01-11
// =============================================================================
class InfrastructureAttribute : public kernel::Infrastructure_ABC
                              , public Overridable_ABC
                              , public tools::Observer_ABC
                              , public kernel::Serializable_ABC
                              , public kernel::ModesObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InfrastructureAttribute( kernel::Controllers& controllers, kernel::UrbanObject_ABC& object,
                                      kernel::PropertiesDictionary& dictionary );
             InfrastructureAttribute( xml::xistream& xis, kernel::Controllers& controllers, kernel::UrbanObject_ABC& object,
                                      kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes );
    virtual ~InfrastructureAttribute();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsEnabled() const;
    virtual const std::string& GetInvalidType() const;
    virtual unsigned int GetThreshold() const;
    virtual const kernel::InfrastructureType* GetType() const;
    virtual void SetType( kernel::InfrastructureType* infrastructure );
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual bool IsOverriden() const;
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void Update( xml::xistream& xis );
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name ModesObserver_ABC
    //@{
    virtual void NotifyModeChanged( int newMode );
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateDictionnary();
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controllers&          controllers_;
    kernel::PropertiesDictionary& dictionary_;
    kernel::InfrastructureType*   type_;
    kernel::UrbanObject_ABC&      object_;
    bool                          enabled_;
    unsigned int                  threshold_;
    std::string                   role_;
    std::string                   invalidType_;
    const geometry::Point2f       position_;
    //@}
};

#endif // __InfrastructureAttribute_h_
