// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PointListParameter_h_
#define __MIL_PointListParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_PointListParameter
    @brief  MIL_PointListParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_PointListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_PointListParameter( const ASN1T_PointList& );
    explicit MIL_PointListParameter( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList );
    virtual ~MIL_PointListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToPointList( ASN1T_PointList& ) const;
    virtual bool ToPointList( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PointListParameter( const MIL_PointListParameter& );            //!< Copy constructor
    MIL_PointListParameter& operator=( const MIL_PointListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< MT_Vector2D > > pointList_;
    //@}
};

#endif // __MIL_PointListParameter_h_
