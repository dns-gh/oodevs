// *****************************************************************************
//
// $Created: NLD 2003-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/DEC_Model_ABC.h $
// $Author: Nld $
// $Modtime: 11/04/05 14:37 $
// $Revision: 3 $
// $Workfile: DEC_Model_ABC.h $
//
// *****************************************************************************

#ifndef __DEC_Model_ABC_h_
#define __DEC_Model_ABC_h_

#include "MIL.h"

class DIA_Model;
class DEC_Workspace;

// =============================================================================
// Created: NLD 2003-11-24
// =============================================================================
class DEC_Model_ABC
{
    MT_COPYNOTALLOWED( DEC_Model_ABC );
public:
    //! @name Constructors/Destructor
    //@{
    DEC_Model_ABC( const DEC_Workspace& decWorkspace, const std::string& strModel, MIL_InputArchive& input, bool bNeedParsing, const std::string& strBinaryPath, const std::string& strSourcePath );
    virtual ~DEC_Model_ABC();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName      () const;
    const std::string& GetScriptName() const;
    DIA_Model&         GetDIAModel  () const;
    const DIA_Type&    GetDIAType   () const;
    //@}

private:
    //! @name Initialization tools
    //@{
           bool NeedScriptParsing( bool bNeedParsing, const std::string& strArchiveName, const std::string& strFileName, const std::string& strOpenedFileArchiveName ) const;
    static bool FileChanged( const std::string& strFileName, time_t since );
    //@}

private:
    const std::string   strModel_;
    const DIA_Type*     pDIAType_;
    DIA_Model*          pDIAModel_;
    std::string         strScript_;
};

#include "DEC_Model_ABC.inl"

#endif // __DEC_Model_ABC_h_
