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
    explicit MIL_PointParameter( const ASN1T_Point& );
    explicit MIL_PointParameter( const MT_Vector2D& point );
    virtual ~MIL_PointParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToPoint( ASN1T_Point& ) const;
    virtual bool ToPoint( boost::shared_ptr< MT_Vector2D >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PointParameter( const MIL_PointParameter& );            //!< Copy constructor
    MIL_PointParameter& operator=( const MIL_PointParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< MT_Vector2D > pPoint_;
    //@}
};

#endif // __MIL_PointParameter_h_
