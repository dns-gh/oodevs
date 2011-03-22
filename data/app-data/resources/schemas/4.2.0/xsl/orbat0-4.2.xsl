<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fn="http://exslt.org/sets">
	<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
	<xsl:template match="node()|@*">
		<xsl:copy>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="sides">
		<xsl:element name="parties">
			<xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="side">
		<xsl:element name="party">
			<xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="diplomacies">
		<xsl:element name="diplomacy">
			<xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="@side">
		<xsl:attribute name="party"><xsl:value-of select="."/></xsl:attribute>
	</xsl:template>
	<xsl:template match="dotations">
		<xsl:element name="resources">
			<xsl:apply-templates select="node()|@*"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="logistic">
		<xsl:element name="logistics">
			<xsl:apply-templates select="node()|@*" mode="logistic"/>
		</xsl:element>
	</xsl:template>
	<xsl:template match="automat">
		<xsl:copy>
			<xsl:choose>
				<xsl:when test="contains(@type,'TC2')"><xsl:attribute name="logistic-level">tc2</xsl:attribute></xsl:when>
			</xsl:choose>
			<xsl:apply-templates select="node()|@*"/>
		</xsl:copy>
	</xsl:template>
	<xsl:template match="automat" mode="logistic">
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
            <xsl:element name="subordinate">
                <xsl:attribute name="id">
                    <xsl:value-of select="@id"/>
                </xsl:attribute>
            </xsl:element>
        </xsl:element>
	</xsl:template>
    <xsl:template match="dotation">
        <xsl:element name="resource">
            <xsl:apply-templates select="node()|@*"/>
        </xsl:element>
    </xsl:template>
</xsl:stylesheet>
