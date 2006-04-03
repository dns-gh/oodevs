// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObstacle.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 4 $
// $Workfile: ParamObstacle.h $
//
// *****************************************************************************

#ifndef __ParamObstacle_h_
#define __ParamObstacle_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class ParamLocation;

// =============================================================================
/** @class  ParamObstacle
    @brief  ParamObstacle
*/
// Created: APE 2004-05-18
// =============================================================================
class ParamObstacle : public QGroupBox, public Param_ABC
{
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     ParamObstacle( ASN1T_MissionGenObject& asnObject, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional, bool bOutsideData = false );
    ~ParamObstacle();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamObstacle( const ParamObstacle& );
    ParamObstacle& operator=( const ParamObstacle& );
    //@}

private:
    //! @name Member data
    //@{
    std::string             strMenuText_;
    ASN1T_MissionGenObject& asnObject_;

    MT_ValuedComboBox< ASN1T_EnumObjectType >*  pTypeCombo_;
    QComboBox*                                  pUrgencyCombo_;
    QComboBox*                                  pPreliminaryCombo_;
    QComboBox*                                  pPriorityCombo_;
    ParamLocation*      pLocation_;
    //@}
};

#endif // __ParamObstacle_h_
