<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
        
    <xsl:template match="node()|@*">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
        </xsl:copy>
    </xsl:template>
    
    <xsl:template match="parameter[@name='Ordre conduite changer regles engagement civils']">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:if test="@value='2'">
                <xsl:attribute name="value">3</xsl:attribute>
            </xsl:if>
            <xsl:if test="@value='1'">
                <xsl:attribute name="value">2</xsl:attribute>
            </xsl:if>
            <xsl:if test="@value='0'">
                <xsl:attribute name="value">1</xsl:attribute>
            </xsl:if>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="parameter[@name='Obstacle']">
        <xsl:copy>
            <xsl:apply-templates select="node()|@*"/>
            <xsl:if test="parent::node()[@name='Attributes']">
                <xsl:if test="parent::node()/parameter[@name='ActivityTime']/parameter[@name='ActivityTime']/@value">
                    <xsl:element name="parameter">
                        <xsl:attribute name="name">ActivityTime</xsl:attribute>
                        <xsl:attribute name="type">quantity</xsl:attribute>
                        <xsl:attribute name="value"><xsl:value-of select="../parameter[@name='ActivityTime']/parameter[@name='ActivityTime']/@value"/></xsl:attribute>
                    </xsl:element>
                </xsl:if>
            </xsl:if>
        </xsl:copy>
    </xsl:template>

</xsl:stylesheet>
