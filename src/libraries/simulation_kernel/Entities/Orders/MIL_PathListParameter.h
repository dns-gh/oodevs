// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PathListParameter_h_
#define __MIL_PathListParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_PathListParameter
    @brief  MIL_PathListParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_PathListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_PathListParameter( const ASN1T_PathList& );
    virtual ~MIL_PathListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToPathList( ASN1T_PathList& asn ) const;
    virtual bool ToPathList( std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PathListParameter( const MIL_PathListParameter& );            //!< Copy constructor
    MIL_PathListParameter& operator=( const MIL_PathListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > pathList_;
    //@}
};

#endif // __MIL_PathListParameter_h_
