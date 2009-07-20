<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	
	<xsl:template match="Natures">
		<natures>
			<xsl:for-each select="Nature">
				<nature type="{.}"/>
			</xsl:for-each>
		</natures>
	</xsl:template>
	
</xsl:transform>