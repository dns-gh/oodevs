// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_LimaFunction_h_
#define __MIL_LimaFunction_h_

#include "protocol/protocol.h"

// =============================================================================
/** @class  MIL_LimaFunction
    @brief  MIL_LimaFunction
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_LimaFunction
{
public:
    //! @name Types
    //@{
    static const MIL_LimaFunction LD_;
    static const MIL_LimaFunction LCA_;
    static const MIL_LimaFunction LC_;
    static const MIL_LimaFunction LI_;
    static const MIL_LimaFunction LO_;
    static const MIL_LimaFunction LCAR_;
    static const MIL_LimaFunction LR_;
    static const MIL_LimaFunction LDM_;
    static const MIL_LimaFunction LFM_;
    static const MIL_LimaFunction LIA_;
    //@}

public:
    //! @name Factory
    //@{
    static       void              Initialize();
    static       void              Terminate ();
    static const MIL_LimaFunction* Find      ( const Common::MsgLimaOrder_Function& asn );
    static const MIL_LimaFunction* Find      ( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    unsigned int                  GetID   () const;
    Common::MsgLimaOrder_Function GetAsnID() const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MIL_LimaFunction( const Common::MsgLimaOrder_Function& asn, unsigned int nID, const std::string& strName );
    ~MIL_LimaFunction();

     MIL_LimaFunction( const MIL_LimaFunction& );            //!< Copy constructor
     MIL_LimaFunction& operator=( const MIL_LimaFunction& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< Common::MsgLimaOrder_Function, const MIL_LimaFunction* > T_LimaFunctionMap;
    typedef T_LimaFunctionMap::const_iterator                       CIT_LimaFunctionMap;
    //@}

    //! @name Types
    //@{
    enum E_Functions
    {
        eLimaFuncLD     = 0, // Ligne de d�bouch�
        eLimaFuncLCA    = 1, // Ligne de changement d'attitude
        eLimaFuncLC     = 2, // Ligne de coordination
        eLimaFuncLI     = 3, // Ligne � interdire
        eLimaFuncLO     = 4, // Ligne d'objectif
        eLimaFuncLCAR   = 5, // Ligne de coup d'arr�t
        eLimaFuncLR     = 6, // Ligne de recueil
        eLimaFuncLDM    = 7, // Ligne de d�but de mission
        eLimaFuncLFM    = 8, // Ligne de fin de mission
        eLimaFuncLIA    = 9  // Ligne d'identification et de recueil
    };
    //@}

private:
    const Common::MsgLimaOrder_Function nAsnID_;
    const unsigned int                  nID_;
    const std::string                   strName_;

private:
    static T_LimaFunctionMap limaFunctions_;
};

#endif // __MIL_LimaFunction_h_
