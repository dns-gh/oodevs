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
             MIL_ResourceNetworkParameter();
    explicit MIL_ResourceNetworkParameter( const sword::ResourceNetworkElement& message );
    explicit MIL_ResourceNetworkParameter( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
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

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@

private:
    //! @name Member data
    //@{
    boost::shared_ptr< DEC_ResourceNetwork > pResourceNetwork_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ResourceNetworkParameter )

#endif // __MIL_ResourceNetworkParameter_h_
