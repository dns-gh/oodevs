<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:param name="imageDirectory" select="'Images/'"/>
    <xsl:param name="cssFile" select="'./../missionSheets.css'"/>
    <xsl:output method="html" indent="yes" encoding="utf-8"/>
    <xsl:template match="mission-sheet">
        <html>
            <head>
                <title>
                    <xsl:value-of select="@name"/>
                </title>
                <link href="{$cssFile}" rel="stylesheet"/>
                <h1 align="center">Mission : 
                  <xsl:value-of select="@name"/>
                </h1>
            </head>
            <body>
                <table border="1" align="center" width="100%" cellpadding="5">
                    <tr>
                        <xsl:apply-templates select="context"/>
                    </tr>
                    <tr>
                        <xsl:apply-templates select="parameters"/>
                    </tr>
                    <tr>
                        <xsl:apply-templates select="behavior"/>
                    </tr>
                    <tr>
                        <xsl:apply-templates select="specific-cases"/>
                    </tr>
                    <tr>
                        <xsl:apply-templates select="comments"/>
                    </tr>
                    <tr>
                        <xsl:apply-templates select="mission-end"/>
                    </tr>
                </table>
            </body>
        </html>
    </xsl:template>
    <xsl:template match="text">
        <xsl:value-of select="."/>
    </xsl:template>
    <xsl:template match="line">
        <xsl:apply-templates select="*"/>
        <br/>
    </xsl:template>
    <xsl:template match="ul">
        <ul>
            <xsl:apply-templates select="*"/>
        </ul>
    </xsl:template>
    <xsl:template match="li">
        <li>
            <xsl:apply-templates select="*"/>
        </li>
    </xsl:template>
    <xsl:template match="bold">
        <b>
            <xsl:apply-templates select="*"/>
        </b>
    </xsl:template>
    <xsl:template match="italic">
        <i>
            <xsl:apply-templates select="*"/>
        </i>
    </xsl:template>
    <xsl:template match="underlined">
        <u>
            <xsl:apply-templates select="*"/>
        </u>
    </xsl:template>
    <xsl:template match="link">
        <xsl:text disable-output-escaping="yes">

&lt;img alt="</xsl:text>
        <xsl:value-of select="child::text/text()"/>
        <xsl:text disable-output-escaping="yes">" src="</xsl:text>
        <xsl:value-of select="$imageDirectory" disable-output-escaping="yes"/>
        <xsl:value-of select="child::text/text()"/>
        <xsl:text disable-output-escaping="yes">"/&gt;</xsl:text>
    </xsl:template>
    <xsl:template match="parameters">
        <td width="200">
            <h2>Parameters</h2>
        </td>
        <td>
            <h2>Mandatory</h2>
				<xsl:for-each select="parameter">
						<xsl:choose>
							<xsl:when test="@optional">
								<xsl:if test="@optional = 'false'">
									<h3><xsl:value-of select="@name"/></h3>
									<xsl:apply-templates select="*"/>
								</xsl:if>
							</xsl:when>
							<xsl:otherwise>
								<h3><xsl:value-of select="@name"/></h3>
								<xsl:apply-templates select="*"/>
							</xsl:otherwise>
						</xsl:choose>
				</xsl:for-each>
            <h2>Optional</h2>
				<xsl:for-each select="parameter">
						<xsl:if test="@optional">
							<xsl:if test="@optional = 'true'">
								<h3><xsl:value-of select="@name"/></h3>
								<xsl:apply-templates select="*"/>
							</xsl:if>
						</xsl:if>
				</xsl:for-each>
        </td>
    </xsl:template>
    <xsl:template match="context">
        <td border="thin solid #6495ed" text-align="left" padding="2px 10px" width="100">
            <h2>Context</h2>
        </td>
        <td>
            <xsl:apply-templates select="*"/>
        </td>
    </xsl:template>
    <xsl:template match="behavior">
        <td width="200">
            <h2>Behavior</h2>
        </td>
        <td>
            <xsl:apply-templates select="*"/>
        </td>
    </xsl:template>
    <xsl:template match="specific-cases">
        <td width="200">
            <h2>Specific cases</h2>
        </td>
        <td>
            <xsl:apply-templates select="*"/>
        </td>
    </xsl:template>
    <xsl:template match="mission-end">
        <td width="200">
            <h2>End of mission</h2>
        </td>
        <td>
            <xsl:apply-templates select="*"/>
        </td>
    </xsl:template>
    <xsl:template match="comments">
        <td width="200">
            <h2>Comments</h2>
        </td>
        <td>
            <xsl:apply-templates select="*"/>
        </td>
    </xsl:template>
</xsl:stylesheet>
