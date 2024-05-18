#include <iostream>
#include <fstream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

int readxml() {
    // Load the XML file
    std::ifstream file("sid.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    // Parse the XML document
    xml_document<> doc;
    doc.parse<0>(&buffer[0]);

    // Get the root node
    xml_node<>* root = doc.first_node("measCollecFile");

    // Display file header
    std::cout << "File Header:" << std::endl;
    xml_node<>* fileHeader = root->first_node("fileHeader");
    std::cout << "File Format Version: " << fileHeader->first_attribute("fileFormatVersion")->value() << std::endl;
    std::cout << "Vendor Name: " << fileHeader->first_attribute("vendorName")->value() << std::endl;
    std::cout << "DN Prefix: " << fileHeader->first_attribute("dnPrefix")->value() << std::endl;

    // Display measurement data
    std::cout << "\nMeasurement Data:" << std::endl;
    xml_node<>* measData = root->first_node("measData");
    xml_node<>* measInfo = measData->first_node("measInfo");
    for (xml_node<>* measValue = measInfo->first_node("measValue"); measValue; measValue = measValue->next_sibling("measValue")) {
        std::cout << "Measured Object LDN: " << measValue->first_attribute("measObjLdn")->value() << std::endl;
        for (xml_node<>* r = measValue->first_node("r"); r; r = r->next_sibling("r")) {
            std::cout << "Measurement Type: " << r->first_attribute("p")->value() << ", Value: " << r->value() << std::endl;
        }
        // Check if suspect attribute exists
        xml_node<>* suspect = measValue->first_node("suspect");
        if (suspect) {
            std::cout << "Suspect: " << suspect->value() << std::endl;
        }
    }

    // Display file footer
    std::cout << "\nFile Footer:" << std::endl;
    xml_node<>* fileFooter = root->first_node("fileFooter");
    std::cout << "Measurement Collection End Time: " << fileFooter->first_node("measCollec")->first_attribute("endTime")->value() << std::endl;

    return 0;
}
