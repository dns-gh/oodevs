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

private:
    //! @name Copy/Assignment
    //@{
    MIL_PolygonParameter( const MIL_PolygonParameter& );            //!< Copy constructor
    MIL_PolygonParameter& operator=( const MIL_PolygonParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< TER_Localisation > pPolygon_;
    bool valid_;
    //@}
};

#endif // __MIL_PolygonParameter_h_
