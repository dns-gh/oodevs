<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="/">
		<launchers>
			<xsl:apply-templates/>
		</launchers>
	</xsl:template>

	<xsl:template match="text()"/>
	
	<xsl:template match="Lanceur">
		<launcher name="{@nom}">
			<xsl:choose>
				<xsl:when test="Indirect">
					<xsl:attribute name="indirect-fire">
						<xsl:text>true</xsl:text>
					</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:attribute name="indirect-fire">
						<xsl:text>false</xsl:text>
					</xsl:attribute>				
				</xsl:otherwise>
			</xsl:choose>
			<xsl:apply-templates/>
		</launcher>
	</xsl:template>
	
	<xsl:template match="Direct">
		<xsl:for-each select="ModificateursPH/*">
			<ph-modifiers posture="{substring-after (name(), 'TireurEn')}">
				<xsl:for-each select="./*">
					<ph-modifier target-posture="{substring-after (name(), 'CibleEn')}" value="{.}"/>
				</xsl:for-each>
			</ph-modifiers>
		</xsl:for-each>
	</xsl:template>
	
	
</xsl:stylesheet>
