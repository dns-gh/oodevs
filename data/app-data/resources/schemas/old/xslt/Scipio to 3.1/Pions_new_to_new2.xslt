<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="1.0">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="text()"/>
	<xsl:template match="/">
		<units>
			<xsl:apply-templates/>
		</units>
	</xsl:template>
	
   <xsl:template match="unit">
		    <xsl:copy>
			<xsl:copy-of select="@*"/>
		        <xsl:element name="nature">
					<xsl:attribute name="nature-app6"/>
					<xsl:attribute name="level">
						<xsl:value-of select="nature/@level"/>
					</xsl:attribute>
					<xsl:attribute name="atlas-nature">
						<xsl:value-of select="nature/@atlas-nature"/>
					</xsl:attribute>
					<xsl:attribute name="nature">
						<xsl:value-of select="nature/@nature"/>
					</xsl:attribute>
                </xsl:element>
			<xsl:copy-of select="equipments"/>
			<xsl:copy-of select="crew-ranks"/>
			<xsl:copy-of select="logistics"/>
			<xsl:copy-of select="stocks"/>
			<xsl:copy-of select="postures"/>
			<xsl:copy-of select="setup"/>
			<xsl:copy-of select="nbc"/>
			<xsl:copy-of select="distance-before-points"/>
			<xsl:copy-of select="drill-blow"/>
			<xsl:copy-of select="force-ratio"/>
			</xsl:copy>
    </xsl:template> 
    
</xsl:transform>