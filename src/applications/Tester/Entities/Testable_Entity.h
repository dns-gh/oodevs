// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Testable_Entity_h_
#define __Testable_Entity_h_

#include "Types.h"
#include "GenObject.h"
#include "Tools/Path.h"
#include "Tools/Location.h"
#include "Workspace.h"

namespace TEST {

    class Automat;
    class Scheduler;

// =============================================================================
/** @class  Testable_Entity
    @brief  Testable Entity
*/
// Created: SBO 2005-08-16
// =============================================================================
class Testable_Entity
{

public:
    //! @name Constructors/Destructor
    //@{
             Testable_Entity( const Workspace& workspace );
    virtual ~Testable_Entity();
    //@}

    //! @name Accessors
    //@{
    virtual       T_EntityId GetId      () const = 0;
    virtual const Position&  GetPosition() const = 0;
    virtual const Automat*   GetAutomat () const;
    //@}

    //! @name Test Parameters
    //@{
    virtual T_EntityId         GetTestParam_ID                 () const;
    virtual uint               GetTestParam_Direction          () const;
    virtual bool               GetTestParam_Bool               () const;
    virtual int                GetTestParam_Numeric            ( int  nMin = 0, int  nMax = std::numeric_limits< int >::max() ) const;
    virtual uint               GetTestParam_Enumeration        ( uint nMin    , uint nMax ) const;
    virtual Position&          GetTestParam_Point              () const;
    virtual T_PositionVector&  GetTestParam_PointList          () const;
    virtual Path&              GetTestParam_Path               () const;
    virtual T_PathVector&      GetTestParam_PathList           () const;
    virtual T_EntityId         GetTestParam_Agent              () const;
    virtual T_IdVector&        GetTestParam_AgentList          () const;
    virtual T_EntityId         GetTestParam_AgentKnowledge     () const;
    virtual T_IdVector&        GetTestParam_AgentKnowledgeList () const;
    virtual T_EntityId         GetTestParam_Automate           () const;
    virtual T_IdVector&        GetTestParam_AutomateList       () const;
    virtual T_EntityId         GetTestParam_PopulationKnowledge() const;
    virtual T_EntityId         GetTestParam_ObjectKnowledge    () const;
    virtual T_IdVector&        GetTestParam_ObjectKnowledgeList() const;
    virtual GenObject&         GetTestParam_GenObject          () const;
    virtual T_GenObjectVector& GetTestParam_GenObjectList      () const;
    virtual Location&          GetTestParam_Location           () const;
    virtual T_LocationVector&  GetTestParam_LocationList       () const;
    virtual Location&          GetTestParam_Polygon            () const;
    virtual T_LocationVector&  GetTestParam_PolygonList        () const;
    virtual uint               GetTestParam_NatureAtlas        () const;
    virtual uint               GetTestParam_GDH                () const;
    virtual T_IdVector&        GetTestParam_MedicalPriorities  () const;
    virtual uint               GetTestParam_DotationDType      () const;

    virtual T_IdVector&        GetTestParam_Limas              () const;
    virtual T_EntityId         GetTestParam_LeftLimit          ();
    virtual T_EntityId         GetTestParam_RightLimit         ();
    //@}

    //! @name Operations
    //@{
    bool MustRecomplete( const Scheduler& scheduler );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Testable_Entity( const Testable_Entity& );            //!< Copy constructor
    Testable_Entity& operator=( const Testable_Entity& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    const Workspace& workspace_;
    T_EntityId       nLeftLimit_;
    T_EntityId       nRightLimit_;
    unsigned int     nLastRecompletion_;
    //@}

};

} // end namespace TEST

#endif // __Testable_Entity_h_
