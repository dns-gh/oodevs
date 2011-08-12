<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="1.0">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:template match="text()"/>
	<xsl:template match="/">
		<models>
		<units>
			<xsl:apply-templates/>
			</units>
		</models>
	</xsl:template>
	
   <xsl:template match="unit">
		    <xsl:copy>
					<xsl:attribute name="masalife">false</xsl:attribute>
			<xsl:copy-of select="node()|@*"/>
			</xsl:copy>
    </xsl:template> 
    
</xsl:transform>