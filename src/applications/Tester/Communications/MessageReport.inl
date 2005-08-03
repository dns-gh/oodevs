// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/MessageReport.inl $
// $Author: Sbo $
// $Modtime: 20/07/05 18:08 $
// $Revision: 1 $
// $Workfile: MessageReport.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: MessageReport::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const MessageReport& MessageReport::Find( const std::string& strName )
{
    CIT_MessageReportMap it = messages_.find( strName );
    return it == messages_.end() ? notDefined : *it->second;
}

// -----------------------------------------------------------------------------
// Name: MessageReport::Find
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const MessageReport& MessageReport::Find( uint asnId )
{
    for( CIT_MessageReportMap it = messages_.begin(); it != messages_.end(); ++it )
        if( it->second->GetAsnID() == asnId )
            return *it->second;
    return notDefined;
}

// -----------------------------------------------------------------------------
// Name: MessageReport::GetName
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
const std::string& MessageReport::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MessageReport::GetID
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
uint MessageReport::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: MessageReport::GetAsnID
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
inline
uint MessageReport::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: MessageReport::GetSubType
// Created: SBO 2005-07-13
// -----------------------------------------------------------------------------
inline
const CommunicationType::ESubType MessageReport::GetSubType() const
{
    return eSubType_;
}

} // end namespace TIC