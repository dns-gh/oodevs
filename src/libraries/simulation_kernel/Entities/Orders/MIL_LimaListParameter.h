// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_LimaListParameter_h_
#define __MIL_LimaListParameter_h_

#include "MIL_BaseParameter.h"

class MIL_LimaOrder;

// =============================================================================
/** @class  MIL_LimaListParameter
@brief  MIL_LimaListParameter
*/
// Created: MGD 2010-10-27
// =============================================================================
class MIL_LimaListParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_LimaListParameter( const Common::MsgLimasOrder& message );
    virtual ~MIL_LimaListParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToLima( Common::MsgLimasOrder& message ) const;
    virtual bool ToLimaList( Common::MsgLimasOrder& message ) const;
    virtual bool ToLima( boost::shared_ptr< TER_Localisation >& ) const;
    virtual bool ToLimaList( std::vector< boost::shared_ptr< TER_Localisation > >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_LimaListParameter( const MIL_LimaListParameter& );            //!< Copy constructor
    MIL_LimaListParameter& operator=( const MIL_LimaListParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< boost::shared_ptr< MIL_LimaOrder > > limas_;
    //@}
};

#endif // __MIL_LimaListParameter_h_
