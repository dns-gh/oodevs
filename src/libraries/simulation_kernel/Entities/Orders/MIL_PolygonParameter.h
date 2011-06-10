// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PolygonParameter_h_
#define __MIL_PolygonParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_PolygonParameter
    @brief  MIL_PolygonParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_PolygonParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PolygonParameter();
    explicit MIL_PolygonParameter( const sword::Polygon& );
    virtual ~MIL_PolygonParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToPolygon( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    virtual bool IsValid() const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive& file, const unsigned int );
    void save( MIL_CheckPointOutArchive& file, const unsigned int ) const;
    //@

private:
    //! @name Member data
    //@{
    boost::shared_ptr< TER_Localisation > pPolygon_;
    bool valid_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PolygonParameter )

#endif // __MIL_PolygonParameter_h_
