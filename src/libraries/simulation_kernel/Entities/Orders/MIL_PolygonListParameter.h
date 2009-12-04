// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_PolygonListParameter_h_
#define __MIL_PolygonListParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_PolygonListParameter
    @brief  MIL_PolygonListParameter
*/
// Created: LDC 2009-05-22
// =============================================================================
class MIL_PolygonListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_PolygonListParameter();
    explicit MIL_PolygonListParameter( const ASN1T_PolygonList& );
    explicit MIL_PolygonListParameter( const std::vector< boost::shared_ptr< TER_Localisation > >& );
    virtual ~MIL_PolygonListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}
    
    //! @name Conversions
    //@{
    virtual bool ToPolygonList( ASN1T_PolygonList& ) const;
    virtual bool ToPolygonList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    //@}

    //! @name Modifiers
    //@{
    virtual void Append( boost::shared_ptr< TER_Localisation > pLocation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_PolygonListParameter( const MIL_PolygonListParameter& );            //!< Copy constructor
    MIL_PolygonListParameter& operator=( const MIL_PolygonListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< TER_Localisation > > polygonList_;
    //@}
};

#endif // __MIL_PolygonListParameter_h_
