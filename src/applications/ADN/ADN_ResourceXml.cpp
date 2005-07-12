//*****************************************************************************
//
// $Created: JDY 03-06-20 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ResourceXml.cpp $
// $Author: Ape $
// $Modtime: 10/02/05 16:10 $
// $Revision: 2 $
// $Workfile: ADN_ResourceXml.cpp $
//
//*****************************************************************************
#include "ADN_Pch.h"
#include "ADN_ResourceXml.h"

#include <windows.h>


//-----------------------------------------------------------------------------
// Name: ADN_ResourceXml constructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_ResourceXml::ADN_ResourceXml(const std::string& data)
{
    SetData(data); 
}


//-----------------------------------------------------------------------------
// Name: ADN_ResourceXml constructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_ResourceXml::ADN_ResourceXml( int idres )
{
    Initialize( idres );
}


//-----------------------------------------------------------------------------
// Name: ADN_ResourceXML::~ADN_ResourceXml
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_ResourceXml::~ADN_ResourceXml()
{

}


//-----------------------------------------------------------------------------
// Name: ADN_ResourceXML::getStringVector
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
T_StringVector  ADN_ResourceXml::getStringVector(const std::string& name)
{
    T_StringVector lfields;
    uint i=0, previ=0;
    while ( i< name.size() )
    {
        if ( name[i]=='/' || name[i]=='\\' )
        {
            lfields.push_back(std::string(name.begin()+previ,name.begin()+i));
            previ=i+1;
        }
        ++i;
    }
    lfields.push_back(std::string(name.begin()+previ,name.end()));    
    return lfields;
}

// -----------------------------------------------------------------------------
// Name: ADN_ResourceXml::Initialize
/** @param  idres 
*/
// Created: AGN 2003-12-12
// -----------------------------------------------------------------------------
void ADN_ResourceXml::Initialize( int idres )
{
    SetData((char*)LockResource( LoadResource(GetModuleHandle(NULL),
                                FindResource(GetModuleHandle(NULL),MAKEINTRESOURCE(idres),"xml")
                                )));    
}
