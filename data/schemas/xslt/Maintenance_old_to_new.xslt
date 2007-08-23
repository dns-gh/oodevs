<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="/">
		<maintenance>
			<xsl:apply-templates/>
		</maintenance>
	</xsl:template>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="RegimesTravail">
		<working-schemes>
			<xsl:for-each select="./*">
				<working-scheme type="{name()}" working-time="{@dureeTravail}">
					<xsl:if test="@delaiAvantAvertissement">
						<xsl:attribute name="time-before-warning">
							<xsl:value-of select="@delaiAvantAvertissement"/>
						</xsl:attribute>
					</xsl:if>
				</working-scheme>
			</xsl:for-each>
		</working-schemes>
	</xsl:template>
	
	<xsl:template match="AlertesDisponibiliteMoyens">
		<resource-availability-alerts>
			<xsl:for-each select="./*/*">
				<resource-availability-alert resource="{name()}" availability-threshold="{@pourcentageMoyensDisponibles}"/>
			</xsl:for-each>
		</resource-availability-alerts>
	</xsl:template>
	
</xsl:stylesheet>
