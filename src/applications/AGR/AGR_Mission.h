// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-22 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_Mission.h $
// $Author: Nld $
// $Modtime: 29/04/05 11:40 $
// $Revision: 18 $
// $Workfile: AGR_Mission.h $
//
// *****************************************************************************

#ifndef __AGR_Mission_h_
#define __AGR_Mission_h_

#include "AGR_Types.h"
#include "AGR_Class.h"

class AGR_Member;
class AGR_Workspace;
// =============================================================================
/** @class  AGR_Mission
    @brief  AGR_Mission
*/
// Created: AGN 2004-04-22
// =============================================================================
class AGR_Mission : public AGR_Class
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_Mission( const std::string& strAsnMissionName, const std::string& strModuleName );
    virtual ~AGR_Mission();
    //@}

    //! @name Operations
    //@{
    void Read( MT_XXmlInputArchive& input, const AGR_Workspace& workspace );

    void GenerateMilClassHeader( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;
    void GenerateMilClassCpp   ( const AGR_Workspace& workspace, const std::string& strOutputPath ) const;

    std::string GenerateDiaDefinition() const;
    std::string GenerateMosImplementation() const;
    std::string GenerateMos2Implementation() const;
    std::string GenerateFicheMission() const;
                                                            //!< Name : Mission_Automate_ASS_AcquerirObjectifs
    virtual std::string MilFileName() const;                //!< MIL_AutomateMission_ASS_AcquerirObjectifs
    virtual std::string MilFilePathName() const;            //!< Automate\MIL_AutomateMission_ASS_AcquerirObjectifs
    virtual std::string HumanName() const;                  //!< Automate Ass AcquerirObjectifs
    virtual std::string ASNTypeName() const;                //!< T_Mission_Automate_mission_automate_ass_acquerir_objectifs
    virtual std::string BaseName() const;                   //!< ASS_AcquerirObjectifs
    virtual std::string WeaponName      () const;           //!< ASS
    virtual std::string HumanMissionName() const;           //!< Acquerir Objectifs

    std::string DIATypeName() const;                        //!< T_Mission_Automate_ASS_AcquerirObjectifs
    std::string MRTBehavior() const;                        //!< MIS_Automate_MRT_ASS_AcquerirObjectifs
    std::string ConduiteBehavior() const;                   //!< MIS_Automate_CDT_ASS_AcquerirObjectifs

    


    virtual std::string KnowledgeAccessor() const;
    //@}

    //! @name Accessors
    //@{
    bool IsMissionForAutomate() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_Mission( const AGR_Mission& );            //!< Copy constructor
    AGR_Mission& operator=( const AGR_Mission& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< int, const AGR_Enumeration* >     T_Int_Enumeration_Map;
    typedef T_Int_Enumeration_Map::iterator       IT_Int_Enumeration_Map;
    typedef T_Int_Enumeration_Map::const_iterator CIT_Int_Enumeration_Map;
    //@}

private:
    //! @name Member data
    //@{
    bool bMissionForAutomata_;
    T_Int_Enumeration_Map memberIdEnumMap_; // map of the enumeration names for member with an enumeration type
    //@}
};

#endif // __AGR_Mission_h_
