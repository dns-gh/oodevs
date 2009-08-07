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
    explicit MIL_PathParameter( boost::shared_ptr< MT_Vector2D > point );
    explicit MIL_PathParameter( const ASN1T_Path& );
    explicit MIL_PathParameter( const std::vector< boost::shared_ptr< MT_Vector2D > >& pointList );
    virtual ~MIL_PathParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToPath( ASN1T_Path& asn ) const;
    virtual bool ToPath( std::vector< boost::shared_ptr< MT_Vector2D > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PathParameter( const MIL_PathParameter& );            //!< Copy constructor
    MIL_PathParameter& operator=( const MIL_PathParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< MT_Vector2D > > path_;
    //@}
};

#endif // __MIL_PathParameter_h_
