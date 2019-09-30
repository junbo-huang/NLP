#ifndef OUTPUT_H
#define OUTPUT_H

#define _USE_MATH_DEFINES

#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

class KMLOutput {
private:
	std::ofstream outFile;

private:
	void get_colour(char xi_color, std::string& xo_color)
	{
		switch (xi_color)
		{
		case 'R':			
			xo_color = "ff0000ff";
			break;
		case 'O':			
			xo_color = "ff3232cc";
			break;
		case 'Y':			
			xo_color = "ff00ffff";
			break;
		case 'G':			
			xo_color = "ff00ff00";
			break;
		case 'B':			
			xo_color = "ffff0000";
			break;
		case 'I':			
			xo_color = "ff54082e";
			break;
		case 'V':			
			xo_color = "ff4f2f4f";
			break;
		default:			
			xo_color = "";
		}
	}

public:
	// KML https://developers.google.com/kml/documentation/kml_tut#paths
	//https://developers.google.com/kml/documentation/kmlreference
	void RemoveOutputFile(std::string fileName) {
		remove(fileName.c_str());
	}

	void PrintKMLInitInfo(std::string fileName) {
		outFile.open(fileName);
		std::ostringstream tempOutput;

		tempOutput << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		tempOutput << "<kml xmlns=\"http://www.opengis.net/kml/2.2\"\n xmlns:atom=\"http://www.w3.org/2005/Atom\"\n xmlns:gx=\"http://www.google.com/kml/ext/2.2\">\n";
		tempOutput << "\t<Document>\n";
		//tempOutput << "\t\t<open>1</open>\n";
		//tempOutput << "\t\t<visibility>1</visibility>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLStyle_Random(int width) {
		PrintKMLStyle('X', width);
	}

	void PrintKMLStyle(char ROYGBIV, int width) {
		std::ostringstream tempOutput;
		std::string colourID;
		std::string colourHex;
		std::string colourMode = "normal";
		// https://developers.google.com/kml/documentation/kmlreference#colorstyle		
		switch ( ROYGBIV )
		{
		case 'R':
			colourID = "redLine";
			colourHex = "ff0000ff";
			break;
		case 'O':
			colourID = "orangeLine";
			colourHex = "ff3232cc";
			break;
		case 'Y':
			colourID = "yellowLine";
			colourHex = "ff00ffff";
			break;
		case 'G':
			colourID = "greenLine";
			colourHex = "ff00ff00";
			break;
		case 'B':
			colourID = "blueLine";
			colourHex = "ffff0000";
			break;
		case 'I':
			colourID = "indigoLine";
			colourHex = "ff54082e";
			break;
		case 'V':
			colourID = "violetLine";
			colourHex = "ff4f2f4f";
			break;
		default:
			colourID = "randomLine";
			colourHex = "";
			colourMode = "random";
		}

		tempOutput << "\t\t<Style id=\"" << colourID << "\">\n";
		tempOutput << "\t\t\t<LineStyle>\n";		
		tempOutput << "\t\t\t\t<color>" << colourHex << "</color>\n";
		tempOutput << "\t\t\t\t<colorMode>" << colourMode << "</colorMode>\n";
		tempOutput << "\t\t\t\t<width>" << width << "</width>\n";
		tempOutput << "\t\t\t</LineStyle>\n";
		tempOutput << "\t\t\t<IconStyle>\n";
		tempOutput << "\t\t\t\t<scale>1.3</scale>\n";
		tempOutput << "\t\t\t\t<Icon>\n";
		tempOutput << "\t\t\t\t\t<href>http://earth.google.com/images/kml-icons/track-directional/track-1.png</href>\n";
		tempOutput << "\t\t\t\t</Icon>\n";
		tempOutput << "\t\t\t</IconStyle>\n";	
		tempOutput << "\t\t\t<LabelStyle>\n";
		tempOutput << "\t\t\t\t<scale>0</scale>\n";
		tempOutput << "\t\t\t</LabelStyle>\n";	
		tempOutput << "\t\t</Style>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLSchema() {
		std::ostringstream tempOutput;
		tempOutput << "\t\t<Schema id=\"schema\">\n";
		tempOutput << "\t\t\t<gx:SimpleArrayField name=\"mmsi\" type=\"int\">\n";
		tempOutput << "\t\t\t\t<displayName>mmsi</displayName>\n";
		tempOutput << "\t\t\t</gx:SimpleArrayField>\n";
		tempOutput << "\t\t\t<gx:SimpleArrayField name=\"sog\" type=\"float\">\n";
		tempOutput << "\t\t\t\t<displayName>sog</displayName>\n";
		tempOutput << "\t\t\t</gx:SimpleArrayField>\n";
		tempOutput << "\t\t\t<gx:SimpleArrayField name=\"cog\" type=\"float\">\n";
		tempOutput << "\t\t\t\t<displayName>cog</displayName>\n";
		tempOutput << "\t\t\t</gx:SimpleArrayField>\n";
		tempOutput << "\t\t</Schema>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLInitPlaneInfo(std::string name, std::string description, char ROYGBIV) {
		std::ostringstream tempOutput;
		std::string colourID;

		tempOutput << "\t\t<Placemark>\n";
		tempOutput << "\t\t\t<name>" << name << "</name>\n";
		tempOutput << "\t\t\t<description>" << description << "</description>\n";
		switch ( ROYGBIV )
		{
		case 'R':
			colourID = "redLine";
			break;
		case 'O':
			colourID = "orangeLine";
			break;
		case 'Y':
			colourID = "yellowLine";
			break;
		case 'G':
			colourID = "greenLine";
			break;
		case 'B':
			colourID = "blueLine";
			break;
		case 'I':
			colourID = "indigoLine";
			break;
		case 'V':
			colourID = "violetLine";
			break;
		default:
			colourID = "randomLine";
		}
		
		tempOutput << "\t\t\t<styleUrl>#" << colourID << "</styleUrl>\n";
		tempOutput << "\t\t\t<gx:Track>\n";
		tempOutput << "\t\t\t\t<altitudeMode>clampToSeaFloor</altitudeMode>\n";
		tempOutput << "\t\t\t\t<gx:interpolate>1</gx:interpolate>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLWhen(std::string when) {
		std::ostringstream tempOutput;
		tempOutput << "\t\t\t\t<when>" << when << "</when>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLWhen(time_t currentTime, std::string UTC) {
		std::ostringstream tempOutput;
		struct tm * timeinfo;
		timeinfo = localtime(&currentTime);
		tempOutput << "\t\t\t\t<when>" << timeinfo->tm_year+1900 << "-"
			<< std::setfill('0') << std::setw(2) << timeinfo->tm_mon+1
			<< "-" << std::setfill('0') << std::setw(2) << timeinfo->tm_mday
			<< "T" << std::setfill('0') << std::setw(2) << timeinfo->tm_hour
			<< ":" << std::setfill('0') << std::setw(2) << timeinfo->tm_min
			<< ":" << std::setfill('0') << std::setw(2) << timeinfo->tm_sec << UTC << "</when>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLPoint(double longitude, double latitude, double altitude) {
		std::ostringstream tempOutput;
		tempOutput << "\t\t\t\t<gx:coord>" << std::fixed << std::setprecision(8) << longitude << " " << latitude << " " << altitude << "</gx:coord>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLPoint(std::string name, char color, double longitude, double latitude, double altitude) {
		std::string kmlColor; //todo fill
		get_colour(color, kmlColor);

		std::ostringstream tempOutput;
		tempOutput << "\t\t<Placemark>\n";
		tempOutput << "\t\t\t<name>" << name << "</name>\n";
		tempOutput << "\t\t\t<Style>\n";
		tempOutput << "\t\t\t\t<geomColor>" << kmlColor << "</geomColor>\n";
		tempOutput << "\t\t\t</Style>\n";
		tempOutput << "\t\t\t<Point>\n";
		tempOutput << "\t\t\t\t<coordinates>" << std::fixed << std::setprecision(8) << longitude << " " << latitude << " " << altitude << "</coordinates>\n";
		tempOutput << "\t\t\t</Point>\n";
		tempOutput << "\t\t</Placemark>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLExtendedInitInfo() {
		std::ostringstream tempOutput;

		tempOutput << "\t\t\t\t<ExtendedData>\n";
		tempOutput << "\t\t\t\t\t<SchemaData schemaUrl=\"#schema\">\n";

		outFile << tempOutput.str();
	}

	void PrintKMLSimpleArrayInitInfo(std::string name) {
		std::ostringstream tempOutput;

		tempOutput << "\t\t\t\t\t\t<gx:SimpleArrayData name=\"" << name << "\">\n";

		outFile << tempOutput.str();
	}

	void PrintKMLSimpleArrayInfo(long data) {
		std::ostringstream tempOutput;

		tempOutput << "\t\t\t\t\t\t\t<gx:value>" << data << "</gx:value>\n"; 

		outFile << tempOutput.str();
	}

	void PrintKMLSimpleArrayInfo(double data) {
		std::ostringstream tempOutput;

		tempOutput << "\t\t\t\t\t\t\t<gx:value>" << data << "</gx:value>\n"; 
		//tempOutput << std::fixed << "\t\t\t\t\t\t\t<gx:value>" << data << "</gx:value>\n"; 
		//std::fixed prevents the data from being transformed into scientific notation

		outFile << tempOutput.str();
	}

	void PrintKMLSimpleArrayEndInfo() {
		std::ostringstream tempOutput;

		tempOutput << "\t\t\t\t\t\t</gx:SimpleArrayData>\n";

		outFile << tempOutput.str();
	}

	void PrintKMLExtendedEndInfo() {
		std::ostringstream tempOutput;

		tempOutput << "\t\t\t\t\t</SchemaData>\n";
		tempOutput << "\t\t\t\t</ExtendedData>\n";

		outFile << tempOutput.str();
	}


	void PrintKMLPlaneState(std::string tempOutput) {
		outFile << tempOutput;
	}

	void PrintKMLEndPlaneInfo() {
		std::ostringstream tempOutput;
		tempOutput << "\t\t\t</gx:Track>\n";
		tempOutput << "\t\t</Placemark>\n";
		outFile << tempOutput.str();
	}

	void PrintKMLEndInfo() {
		std::ostringstream tempOutput;
		tempOutput << "\t</Document>\n";
		tempOutput << "</kml>\n";
		outFile << tempOutput.str();
		outFile.close();
	}
};

#endif