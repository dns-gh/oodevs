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
#include "MIL.h"
#include <boost/serialization/export.hpp>

class MIL_Agent_ABC;
class PHY_MaterialCompositionType;

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
    explicit MaterialAttribute( const PHY_MaterialCompositionType& material );
    virtual ~MaterialAttribute();
    //@}

    //! @name CheckPoint
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Operations
    //@{
    const PHY_MaterialCompositionType& GetMaterial() const;
    //@}

    //! @name Operations
    //@{
    MaterialAttribute& operator=( const MaterialAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    // $$$$ _RC_ JSR 2011-02-16: Passer en référence -> load/save_construct_data
    const PHY_MaterialCompositionType* material_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MaterialAttribute )

#endif // __MaterialAttribute_h_
