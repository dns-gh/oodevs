// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ConstructionAttribute_h_
#define __ConstructionAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class NBCAgent;
    class PropertiesDictionary;
    class DotationType;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ConstructionAttribute
    @brief  ConstructionAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ConstructionAttribute : public kernel::ConstructionAttribute_ABC
                            , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ConstructionAttribute( kernel::PropertiesDictionary& dico );
             ConstructionAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~ConstructionAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Accessors
    //@{
    void SetCompletion( unsigned int value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConstructionAttribute( const ConstructionAttribute& );            //!< Copy constructor
    ConstructionAttribute& operator=( const ConstructionAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > completion_;
    const kernel::DotationType*         dotationType_;
    unsigned int                        dotationCount_;
    //@}
};

#endif // __ConstructionAttribute_h_
