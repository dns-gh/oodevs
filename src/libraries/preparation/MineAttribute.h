// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MineAttribute_h_
#define __MineAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class PropertiesDictionary;
    class DotationType;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MineAttribute
    @brief  MineAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class MineAttribute : public kernel::MineAttribute_ABC
                    , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MineAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             MineAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~MineAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetDensity( float density );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MineAttribute( const MineAttribute& );            //!< Copy constructor
    MineAttribute& operator=( const MineAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< double > density_;
    //@}
};

#endif // __MineAttribute_h_
