// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-07 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __AGR_TypeRC_h_
#define __AGR_TypeRC_h_

class AGR_Workspace;
class AGR_Type_ABC;
class AGR_RC;

// =============================================================================
// Created: NLD 2005-09-07
// =============================================================================
class AGR_TypeRC
{
public:
    //! @name Constructors/Destructor
    //@{
     AGR_TypeRC( const std::string& strName, MT_XXmlInputArchive& input, const AGR_Workspace& workspace );
    ~AGR_TypeRC();
    //@}

    //! @name Generators
    //@{
    std::string GenerateHALEnum                 ( const AGR_RC& rcInstance ) const;
    std::string GenerateMILRCClassHeader        ( const AGR_RC& rcInstance ) const;
    std::string GenerateMILRCClassBodyInitialize( const AGR_RC& rcInstance ) const;
    std::string GenerateMILRCClassBodyIncludes  ( const AGR_RC& rcInstance ) const;
    std::string GenerateMOS2                    ( const AGR_RC& rcInstance ) const;
    std::string GenerateMOS                     ( const AGR_RC& rcInstance ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AGR_TypeRC( const AGR_TypeRC& );            //!< Copy constructor
    AGR_TypeRC& operator=( const AGR_TypeRC& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    std::string GenerateEnum() const; 
    //@}

private:
    //! @name Types
    //@{
    class Param
    {
        public:
            Param( MT_XXmlInputArchive& input, const AGR_Workspace& workspace );

                  std::string   strName_;
            const AGR_Type_ABC* pType_;
                  bool          bOptional_;
    };

    typedef std::vector< const Param* >     T_ParamVector;
    typedef T_ParamVector::const_iterator   CIT_ParamVector;
    //@}

private:
          std::string   strName_;
    const std::string   strAsnName_;
          std::string   strMILEnumName_;
          std::string   strHumanName_;
    const AGR_Type_ABC* pType_;     // Type pour CR de type "CR-EnPosteFaceAObstacle ::= KnowledgeObject"
    T_ParamVector       params_; // Type pour CR de type CR-PreparationMission ::= SEQUENCE 
};

#include "AGR_TypeRC.inl"

#endif // __AGR_TypeRC_h_
