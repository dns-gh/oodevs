// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PointParameter_h_
#define __MIL_PointParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_PointParameter
    @brief  MIL_PointParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_PointParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PointParameter();
    explicit MIL_PointParameter( const sword::Point& );
    explicit MIL_PointParameter( const MT_Vector2D& point );
    virtual ~MIL_PointParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
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
    boost::shared_ptr< MT_Vector2D > pPoint_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PointParameter )

#endif // __MIL_PointParameter_h_
