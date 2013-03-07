// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __OccupantAttribute_h_
#define __OccupantAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

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
/** @class  OccupantAttribute
    @brief  OccupantAttribute
*/
// Created: MGD 2010-02-18
// =============================================================================
class OccupantAttribute : public kernel::OccupantAttribute_ABC
                      , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit OccupantAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             OccupantAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~OccupantAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetMaxSize( unsigned int time );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    OccupantAttribute( const OccupantAttribute& );            //!< Copy constructor
    OccupantAttribute& operator=( const OccupantAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int iMaxSize_;
    //@}
};

#endif // __OccupantAttribute_h_
