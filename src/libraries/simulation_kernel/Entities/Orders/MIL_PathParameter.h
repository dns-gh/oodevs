// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PathParameter_h_
#define __MIL_PathParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_PathParameter
    @brief  MIL_PathParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_PathParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PathParameter();
    explicit MIL_PathParameter( boost::shared_ptr< MT_Vector2D > point );
    explicit MIL_PathParameter( const sword::Path& );
    explicit MIL_PathParameter( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList );
    virtual ~MIL_PathParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
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
    std::vector< boost::shared_ptr< MT_Vector2D > > path_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_PathParameter )

#endif // __MIL_PathParameter_h_
