<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:fn="http://exslt.org/sets">

<xsl:key name="logkey" match="logistic/automat" use="@id"/>
<xsl:key name="logsubkey" match="logistic//subordinate" use="@id"/>
<xsl:key name="tackey" match="tactical//formation | tactical//automat" use="@id"/>

<xsl:output method="xml" indent="yes" encoding="ISO-8859-1" standalone="no"/>

<!-- default to copy all -->
<xsl:template match="@*|node()">
    <xsl:copy><xsl:apply-templates select="@*|node()"/></xsl:copy>
</xsl:template>

<!-- process tactical/automat if tc2 -->
<xsl:template match="automat">
    <xsl:copy>
        <xsl:apply-templates select="@*"/>
        <xsl:if test="count(key('logkey',@id)[count(subordinate[@link!='tc2'])=0]) !=0 ">
            <xsl:attribute name="logistic-level">tc2</xsl:attribute>
        </xsl:if>
        <xsl:apply-templates select="node()"/>
    </xsl:copy>
</xsl:template>

<!-- process tactical/formation if logistic-base -->
<xsl:template match="formation">
    <xsl:copy>
        <xsl:apply-templates select="@*"/>
        <xsl:if test="count(key('logkey',./automat/@id)[count(subordinate[@link!='tc2'])!=0]) !=0 ">
            <xsl:attribute name="logistic-level">logistic-base</xsl:attribute>
        </xsl:if>
        <xsl:apply-templates select="node()"/>
    </xsl:copy>
</xsl:template>

<!-- replace logistic node with logistics node-->
<xsl:template match="logistic">
<xsl:element name="logistics">
    <xsl:apply-templates select="automat[count(subordinate[@link!='tc2'])=0]" mode="logistic"/>
    <!-- process logistic/automat if tc2 -->
    <!-- process subordinates which are tc2 -->
    <xsl:for-each select="fn:distinct(automat[count(subordinate[@link!='tc2'])!=0]/subordinate[count(key('logkey',@id)[count(subordinate[@link!='tc2'])=0]) !=0]/@id)/..">
        <xsl:variable name="parentId" select="../@id"/>
        <xsl:variable name="currentId" select="@id"/>
        <xsl:if test="count(key('logkey',$currentId)[count(subordinate[@link!='tc2'])=0]) !=0">
            <xsl:element name="logistic-base">
                <xsl:attribute name="id"><xsl:value-of select="key('tackey',$parentId)/../@id"/></xsl:attribute>
                <xsl:element name="subordinate">
                    <xsl:attribute name="id"><xsl:value-of select="$currentId"/></xsl:attribute>
                    <xsl:apply-templates select="key('logsubkey',$currentId)/quotas"/>
                </xsl:element>
            </xsl:element>
        </xsl:if>
    </xsl:for-each>
    <!-- process subordinates which are logistic-base -->
    <xsl:for-each select="key('tackey',automat[count(subordinate[@link!='tc2'])!=0]/subordinate[count(key('logkey',@id)[count(subordinate[@link!='tc2'])!=0]) !=0]/@id)/..">
        <xsl:element name="logistic-base">
            <!-- find a subordinate non tc2 and retrieve the logistic superior -->
            <xsl:for-each select="key('tackey',key('logsubkey',automat[count(key('logkey',@id)[count(subordinate[@link!='tc2'])!=0]) !=0]/@id)[@link!='tc2']/../@id)/..">
                <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
            </xsl:for-each>
            <xsl:element name="subordinate">
                <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
                <xsl:apply-templates select="key('logsubkey',automat/@id)/quotas"/>
            </xsl:element>
        </xsl:element>
    </xsl:for-each>
</xsl:element>
</xsl:template>

<!-- process logistic/automat if tc2 -->
<xsl:template match="automat[count(subordinate[@link!='tc2'])=0]" mode="logistic">
<xsl:element name="tc2">
    <xsl:attribute name="id">
        <xsl:value-of select="@id"/>
    </xsl:attribute>
    <xsl:for-each select="subordinate">
        <xsl:element name="subordinate">
            <xsl:attribute name="id">
                <xsl:value-of select="@id"/>
            </xsl:attribute>
        </xsl:element>  
    </xsl:for-each>
</xsl:element>
</xsl:template>

</xsl:stylesheet>
