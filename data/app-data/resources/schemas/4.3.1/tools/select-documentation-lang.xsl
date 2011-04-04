<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml" xmlns:xs="http://www.w3.org/2001/XMLSchema">
	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<!-- Extract only xs:documentation with xml:lang='en' and copy child nodes and attributes -->
	<xsl:template match="*[name()!='xs:documentation' or not(@xml:lang) or @xml:lang='en']">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template>
	
	<!-- direct copy of attributes -->
	<xsl:template match="@*">
		<xsl:copy-of select="."/>
	</xsl:template>
	
	<!-- do nothing with the rest -->
	<xsl:template match="*"/>

</xsl:stylesheet>