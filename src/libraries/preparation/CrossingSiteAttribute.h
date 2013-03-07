// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSiteAttribute_h_
#define __CrossingSiteAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class PropertiesDictionary;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  CrossingSiteAttribute
    @brief  CrossingSiteAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class CrossingSiteAttribute : public kernel::CrossingSiteAttribute_ABC
                            , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSiteAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             CrossingSiteAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~CrossingSiteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetWidth( unsigned int value );
    void SetDepth( unsigned int value );
    void SetFlowSpeed( unsigned int value );
    void SetConstruction( bool need );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CrossingSiteAttribute( const CrossingSiteAttribute& );            //!< Copy constructor
    CrossingSiteAttribute& operator=( const CrossingSiteAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

public:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > width_;
    kernel::UnitedValue< unsigned int > depth_;
    kernel::UnitedValue< unsigned int > speed_;
    bool needsConstruction_;
    //@}
};

#endif // __CrossingSiteAttribute_h_
