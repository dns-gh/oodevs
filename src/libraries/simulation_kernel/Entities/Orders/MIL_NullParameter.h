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
    virtual bool ToPointList( ASN1T_PointList& ) const;
    virtual bool ToPolygonList( ASN1T_PolygonList& ) const;
    virtual bool ToAgentKnowledgeList( ASN1T_UnitKnowledgeList& asn ) const;
    virtual bool ToAgentList( ASN1T_UnitList& asn ) const;
    virtual bool ToAutomatList( ASN1T_AutomatList& ) const;
    virtual bool ToGenObjectList( ASN1T_PlannedWorkList& ) const;
    virtual bool ToLocationList( ASN1T_LocationList& ) const;
    virtual bool ToObjectiveList( ASN1T_MissionObjectiveList& asn ) const;
    virtual bool ToObjectKnowledgeList( ASN1T_ObjectKnowledgeList& asn ) const;
    virtual bool ToPathList( ASN1T_PathList& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_NullParameter( const MIL_NullParameter& );            //!< Copy constructor
    MIL_NullParameter& operator=( const MIL_NullParameter& ); //!< Assignment operator
    //@}
};

#endif // __MIL_NullParameter_h_
