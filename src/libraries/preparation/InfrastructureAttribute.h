// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InfrastructureAttribute_h_
#define __InfrastructureAttribute_h_

#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
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
                              , public kernel::Serializable_ABC
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
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void Update( xml::xistream& xis );
    //@}

    //! @name Serializable_ABC
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateDictionnary( bool changed = false );
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
    std::string                   invalidType_;
    const geometry::Point2f       position_;
    const QString                 typeProperty_;
    const QString                 enableProperty_;
    const QString                 thresholdProperty_;
    //@}
};

#endif // __InfrastructureAttribute_h_
