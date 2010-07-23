// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MaterialAttribute_h_
#define __MaterialAttribute_h_

#include "ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace urban
{
    class MaterialCompositionType;
}
class MIL_Agent_ABC;

// =============================================================================
/** @class  MaterialAttribute
    @brief  MaterialAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class MaterialAttribute : public ObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MaterialAttribute();
    explicit MaterialAttribute( urban::MaterialCompositionType& material );
    virtual ~MaterialAttribute();
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Operations
    //@{
    urban::MaterialCompositionType& GetMaterial() const;
    //@}

    //! @name Operations
    //@{
    MaterialAttribute& operator=( const MaterialAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    urban::MaterialCompositionType* material_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MaterialAttribute )

#endif // __MaterialAttribute_h_
