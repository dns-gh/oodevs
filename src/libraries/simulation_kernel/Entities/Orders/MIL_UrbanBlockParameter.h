// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanBlockParameter_h_
#define __MIL_UrbanBlockParameter_h_

#include "MIL_BaseParameter.h"

class MIL_EntityManager_ABC;
class UrbanObjectWrapper;

namespace sword
{
    class UrbanObjectKnowledgeId;
}

// =============================================================================
/** @class  MIL_UrbanBlockParameter
    @brief  MIL_UrbanBlockParameter
*/
// Created: MGD 2009-11-02
// =============================================================================
class MIL_UrbanBlockParameter : public MIL_BaseParameter
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanBlockParameter( const sword::UrbanObjectKnowledgeId& asn, MIL_EntityManager_ABC& entityManager );
    explicit MIL_UrbanBlockParameter( boost::shared_ptr< UrbanObjectWrapper > pUrbanBlock );
    virtual ~MIL_UrbanBlockParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( MIL_ParameterType_ABC::E_Type type ) const;
    //@}

    //! @name Conversions
    //@{
    virtual bool ToUrbanBlock( boost::shared_ptr< UrbanObjectWrapper >& ) const;
    virtual bool ToElement( sword::MissionParameter_Value& elem ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_UrbanBlockParameter( const MIL_UrbanBlockParameter& );            //!< Copy constructor
    MIL_UrbanBlockParameter& operator=( const MIL_UrbanBlockParameter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< UrbanObjectWrapper > pUrbanBlock_;
    //@}
};

#endif // __MIL_UrbanBlockParameter_h_
