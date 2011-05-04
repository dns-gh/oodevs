// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ResourceNetworkParameter_h_
#define __MIL_ResourceNetworkParameter_h_

#include "MIL_BaseParameter.h"

namespace sword
{
    class ResourceNetworkElement;
}

// =============================================================================
/** @class  MIL_ResourceNetworkParameter
    @brief  MIL_ResourceNetworkParameter
*/
// Created: JSR 2011-05-03
// =============================================================================
class MIL_ResourceNetworkParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_ResourceNetworkParameter( const sword::ResourceNetworkElement& message );
    virtual ~MIL_ResourceNetworkParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToResourceNetwork( boost::shared_ptr< DEC_ResourceNetwork >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}


private:
    //! @name Copy/Assignment
    //@{
    MIL_ResourceNetworkParameter( const MIL_ResourceNetworkParameter& );            //!< Copy constructor
    MIL_ResourceNetworkParameter& operator=( const MIL_ResourceNetworkParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_ResourceNetwork > pResourceNetwork_;
    //@}
};

#endif // __MIL_ResourceNetworkParameter_h_
