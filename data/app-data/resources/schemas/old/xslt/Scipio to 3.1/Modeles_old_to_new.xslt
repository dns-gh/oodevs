<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<models>
			<xsl:apply-templates/>
		</models>
	</xsl:template>
	
	<xsl:template match="Pions">
		<units>
			<xsl:for-each select="Modele">
				<unit name="{@nom}" dia-type="{DIAType}" file="{File}">
					<xsl:apply-templates/>
				</unit>
			</xsl:for-each>
		</units>
	</xsl:template>
	
	<xsl:template match="Missions">
		<missions>
			<xsl:apply-templates/>
		</missions>
	</xsl:template>
	
	<xsl:template match="Mission">
		<mission><xsl:if test="@nom">
				<xsl:attribute name="name">
					<xsl:value-of select="@nom"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:for-each select="OrdresConduite/OrdreConduite">
				<fragorder name="{@nom}"/>
			</xsl:for-each>
		</mission>
	</xsl:template>

	<xsl:template match="Automates">
		<automats><xsl:for-each select="Modele">
				<automat name="{@nom}" dia-type="{DIAType}" file="{File}">
					<xsl:apply-templates/>
				</automat>
			</xsl:for-each>
		</automats>
	</xsl:template>
	
	<xsl:template match="Populations">
		<populations><xsl:for-each select="Modele">
				<population name="{@nom}" dia-type="{DIAType}" file="{File}">
					<xsl:apply-templates/>
				</population>
			</xsl:for-each>
		</populations>
	</xsl:template>
	
	<xsl:template match="Missions">
		<missions><xsl:for-each select="Mission">
				<mission name="{@nom}">
					<xsl:for-each select="OrdresConduite/OrdreConduite">
						<fragorder name="{@nom}"/>
					</xsl:for-each>
				</mission>
			</xsl:for-each>
		</missions>
	</xsl:template>
	
	
</xsl:transform>
