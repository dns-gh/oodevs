// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef InputPropagationAttribute_h
#define InputPropagationAttribute_h

#include "ObjectAttribute_ABC.h"
#include "UpdatableAttribute_ABC.h"
#include "MIL.h"

namespace xml
{
    class xistream;
}

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  InputPropagationAttribute
    @brief  Input propagation attribute
*/
// Created: LGY 2012-10-05
// =============================================================================
class InputPropagationAttribute : public ObjectAttribute_ABC
                                , public UpdatableAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InputPropagationAttribute();
    explicit InputPropagationAttribute( xml::xistream& xis );
    virtual ~InputPropagationAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

private:
    //! @name Member data
    //@{
    std::string model_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( InputPropagationAttribute )

#endif // InputPropagationAttribute_h
