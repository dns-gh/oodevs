// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ResourceNetworkNodeParameter_h_
#define __MIL_ResourceNetworkNodeParameter_h_

#include "MIL_BaseParameter.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;

// =============================================================================
/** @class  MIL_ResourceNetworkNodeParameter
    @brief  MIL_ResourceNetworkNodeParameter
*/
// Created: ABR 2012-02-15
// =============================================================================
class MIL_ResourceNetworkNodeParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_ResourceNetworkNodeParameter();
    explicit MIL_ResourceNetworkNodeParameter( const sword::ResourceNetworkElement& message );
    explicit MIL_ResourceNetworkNodeParameter( boost::shared_ptr< DEC_ResourceNetwork > resourceNetwork );
    virtual ~MIL_ResourceNetworkNodeParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToResourceNetworkNode( boost::shared_ptr< DEC_ResourceNetwork >& ) const;
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
    boost::shared_ptr< DEC_ResourceNetwork > pResourceNetwork_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MIL_ResourceNetworkNodeParameter )

#endif // __MIL_ResourceNetworkNodeParameter_h_
