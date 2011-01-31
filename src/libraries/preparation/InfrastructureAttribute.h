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

#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include <map>

namespace kernel
{
    class Displayer_ABC;
    class InfrastructureType;
    class PropertiesDictionary;
}

namespace gui
{
    class TerrainObjectProxy;
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
    explicit InfrastructureAttribute( const gui::TerrainObjectProxy& object, const kernel::InfrastructureType& infrastructureType, kernel::PropertiesDictionary& dico );
             InfrastructureAttribute( const gui::TerrainObjectProxy& object, xml::xistream& xis, const kernel::InfrastructureType& infrastructureType, kernel::PropertiesDictionary& dico );
    virtual ~InfrastructureAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    void Update( xml::xistream& xis );
    void Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    bool IsEnabled() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InfrastructureAttribute( const InfrastructureAttribute& );            //!< Copy constructor
    InfrastructureAttribute& operator=( const InfrastructureAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    const kernel::InfrastructureType& type_;
    bool enabled_;
    float threshold_;
    std::string role_;
    const gui::TerrainObjectProxy& object_;
    //@}
};

#endif // __InfrastructureAttribute_h_
