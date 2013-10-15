// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoofShapeType_h_
#define __PHY_RoofShapeType_h_


// =============================================================================
/** @class  PHY_RoofShapeType
    @brief  PHY_RoofShapeType
*/
// Created: JSR 2011-02-16
// =============================================================================
class PHY_RoofShapeType : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_RoofShapeType* > T_RoofShapeMap;
    typedef T_RoofShapeMap::const_iterator CIT_RoofShapeMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    static const PHY_RoofShapeType* Find( const std::string& strName );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RoofShapeType( const std::string& strName );
    virtual ~PHY_RoofShapeType();
    //@}

    //! @name Helpers
    //@{
    static void ReadRoofShape( xml::xistream& xis );
    //@}

private:
    //! @name Static data
    //@{
    static T_RoofShapeMap roofShapes_;
    std::string strName_;
    //@}
};

#endif // __PHY_RoofShapeType_h_
