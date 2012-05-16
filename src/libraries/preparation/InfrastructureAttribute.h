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

#include "Overridable_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
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
                              , public Overridable_ABC
                              , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             InfrastructureAttribute( xml::xistream& xis, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dico,
                                      const kernel::ObjectTypes& objectTypes );
    virtual ~InfrastructureAttribute();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsEnabled() const;
    virtual bool HasValidType() const;
    virtual unsigned int GetThreshold() const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void SetOverriden( bool& overriden ) const;
    virtual void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    void Update( xml::xistream& xis );
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    const kernel::InfrastructureType* type_;
    bool enabled_;
    unsigned int threshold_;
    std::string role_;
    const geometry::Point2f position_;
    //@}
};

#endif // __InfrastructureAttribute_h_
