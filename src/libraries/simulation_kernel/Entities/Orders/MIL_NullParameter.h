// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MIL_NullParameter_h_
#define __MIL_NullParameter_h_

#include "MIL_BaseParameter.h"

// =============================================================================
/** @class  MIL_NullParameter
    @brief  MIL_NullParameter
*/
// Created: LDC 2009-05-20
// =============================================================================
class MIL_NullParameter : public MIL_BaseParameter
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_NullParameter();
    virtual ~MIL_NullParameter();
    //@}

    //! @name Type checking
    //@{
    virtual bool IsOfType( const MIL_ParameterType_ABC& ) const;
    //@}

    //! @name Conversions
    //@{
    // Conversions to empty lists are allowed.
    virtual bool ToPointList( Common::MsgPointList& ) const;
    virtual bool ToPolygonList( Common::MsgPolygonList& ) const;
    virtual bool ToAgentKnowledgeList( Common::MsgUnitKnowledgeList& asn ) const;
    virtual bool ToAgentList( Common::MsgUnitList& asn ) const;
    virtual bool ToAutomatList( Common::MsgAutomatList& ) const;
    virtual bool ToGenObjectList( Common::MsgPlannedWorkList& ) const;
    virtual bool ToLocationList( Common::MsgLocationList& ) const;
    virtual bool ToObjectiveList( Common::MsgMissionObjectiveList& asn ) const;
    virtual bool ToObjectKnowledgeList( Common::MsgObjectKnowledgeList& asn ) const;
    virtual bool ToPathList( Common::MsgPathList& asn ) const;
    virtual bool ToNatureAtlas( Common::MsgAtlasNature& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_NullParameter( const MIL_NullParameter& );            //!< Copy constructor
    MIL_NullParameter& operator=( const MIL_NullParameter& ); //!< Assignment operator
    //@}
};

#endif // __MIL_NullParameter_h_
