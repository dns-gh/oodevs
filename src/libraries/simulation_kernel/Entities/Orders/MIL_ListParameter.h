// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_ListParameter_h_
#define __MIL_ListParameter_h_

#include "MIL_BaseParameter.h"
#include "protocol/protocol.h"

class DEC_KnowledgeResolver_ABC;

// =============================================================================
/** @class  MIL_ListParameter
    @brief  MIL_ListParameter
*/
// Created: LDC 2010-09-21
// =============================================================================
class MIL_ListParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const ::google::protobuf::RepeatedPtrField< ::Common::MsgMissionParameter_Value >& list );
             MIL_ListParameter( const DEC_KnowledgeResolver_ABC& resolver, const std::vector< boost::shared_ptr< DEC_Knowledge_Urban > >& urbanBlockList );
    virtual ~MIL_ListParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    
    virtual bool ToList( std::vector< Common::MsgMissionParameter_Value >& ) const;
    virtual bool ToList( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_ListParameter( const MIL_ListParameter& );            //!< Copy constructor
    MIL_ListParameter& operator=( const MIL_ListParameter& ); //!< Assignment operator
    //@}
    
    typedef std::vector< Common::MsgMissionParameter_Value > T_ParameterList;
    typedef T_ParameterList::const_iterator                  CIT_ParameterList;

    T_ParameterList list_;
    const DEC_KnowledgeResolver_ABC& resolver_;
};

#endif // __MIL_ListParameter_h_
