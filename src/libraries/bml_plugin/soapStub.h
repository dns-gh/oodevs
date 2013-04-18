/* soapStub.h
   Generated by gSOAP 2.7.10 from DCSOperations.h
   Copyright(C) 2000-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapStub_H
#define soapStub_H
#include <vector>
#include "stdsoap2.h"

/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not redeclare here */

#endif

#if 0 /* volatile type: do not redeclare here */

#endif

#ifndef SOAP_TYPE_ns1__pushOrderResponse
#define SOAP_TYPE_ns1__pushOrderResponse (11)
/* ns1:pushOrderResponse */
struct ns1__pushOrderResponse
{
public:
    std::string _pushOrderReturn;    /* SOAP 1.2 RPC return element (when namespace qualified) */    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pushOrder
#define SOAP_TYPE_ns1__pushOrder (12)
/* ns1:pushOrder */
struct ns1__pushOrder
{
public:
    std::string _orderPush;    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pushReportResponse
#define SOAP_TYPE_ns1__pushReportResponse (14)
/* ns1:pushReportResponse */
struct ns1__pushReportResponse
{
public:
    std::string _pushReportReturn;    /* SOAP 1.2 RPC return element (when namespace qualified) */    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pushReport
#define SOAP_TYPE_ns1__pushReport (15)
/* ns1:pushReport */
struct ns1__pushReport
{
public:
    std::string _reportPush;    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pullOrderResponse
#define SOAP_TYPE_ns1__pullOrderResponse (17)
/* ns1:pullOrderResponse */
struct ns1__pullOrderResponse
{
public:
    std::string _pullOrderReturn;    /* SOAP 1.2 RPC return element (when namespace qualified) */    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pullOrder
#define SOAP_TYPE_ns1__pullOrder (18)
/* ns1:pullOrder */
struct ns1__pullOrder
{
public:
    std::string _orderPull;    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pullReportResponse
#define SOAP_TYPE_ns1__pullReportResponse (20)
/* ns1:pullReportResponse */
struct ns1__pullReportResponse
{
public:
    std::string _pullReportReturn;    /* SOAP 1.2 RPC return element (when namespace qualified) */    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_ns1__pullReport
#define SOAP_TYPE_ns1__pullReport (21)
/* ns1:pullReport */
struct ns1__pullReport
{
public:
    std::string _reportPull;    /* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (22)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
    char dummy;    /* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (23)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
    char *SOAP_ENV__Value;    /* optional element of type xsd:QName */
    struct SOAP_ENV__Code *SOAP_ENV__Subcode;    /* optional element of type SOAP-ENV:Code */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (25)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
    int __type;    /* any type of element <fault> (defined below) */
    void *fault;    /* transient */
    char *__any;
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (28)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
    char *SOAP_ENV__Text;    /* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (29)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
    char *faultcode;    /* optional element of type xsd:QName */
    char *faultstring;    /* optional element of type xsd:string */
    char *faultactor;    /* optional element of type xsd:string */
    struct SOAP_ENV__Detail *detail;    /* optional element of type SOAP-ENV:Detail */
    struct SOAP_ENV__Code *SOAP_ENV__Code;    /* optional element of type SOAP-ENV:Code */
    struct SOAP_ENV__Reason *SOAP_ENV__Reason;    /* optional element of type SOAP-ENV:Reason */
    char *SOAP_ENV__Node;    /* optional element of type xsd:string */
    char *SOAP_ENV__Role;    /* optional element of type xsd:string */
    struct SOAP_ENV__Detail *SOAP_ENV__Detail;    /* optional element of type SOAP-ENV:Detail */
};
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif


/******************************************************************************\
 *                                                                            *
 * Typedef Synonyms                                                           *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of soapStub.h */