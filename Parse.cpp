#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include "rapidxml.hpp"

using namespace rapidxml;

// Define the structure for an attribute
struct Attribute {
    std::string name;
    std::string value;
};

// Define the structure for a node
struct Node {
    std::string name;
    std::string value;
    std::vector<Attribute> attributes;
    std::vector<std::shared_ptr<Node>> children;
};

// Function to create the tree structure from the XML node
std::shared_ptr<Node> CreateTree(xml_node<>* xmlNode) {
    auto node = std::make_shared<Node>();
    node->name = xmlNode->name();
    if (xmlNode->value()) {
        node->value = xmlNode->value();
    }

    for (xml_attribute<>* attr = xmlNode->first_attribute(); attr; attr = attr->next_attribute()) {
        Attribute attribute;
        attribute.name = attr->name();
        attribute.value = attr->value();
        node->attributes.push_back(attribute);
    }

    for (xml_node<>* child = xmlNode->first_node(); child; child = child->next_sibling()) {
        node->children.push_back(CreateTree(child));
    }

    return node;
}

// Function to display the tree structure
void DisplayTree(const std::shared_ptr<Node>& node, int depth = 0) {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << node->name;

    for (const auto& attr : node->attributes) {
        std::cout << " [" << attr.name << "=" << attr.value << "]";
    }

    if (!node->value.empty()) {
        std::cout << ": " << node->value;
    }

    std::cout << std::endl;

    for (const auto& child : node->children) {
        DisplayTree(child, depth + 1);
    }
}

int main() {
    // Load the XML file into a vector of chars
    std::ifstream file("measDataCollection.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0'); // Null-terminate the buffer

    // Parse the XML document
    xml_document<> doc;
    try {
        doc.parse<0>(&buffer[0]);
    }
    catch (const rapidxml::parse_error& e) {
        std::cerr << "XML parse error: " << e.what() << std::endl;
        return 1;
    }

    // Get the root node
    xml_node<>* root = doc.first_node("measCollecFile");

    if (!root) {
        std::cerr << "No root node found!" << std::endl;
        return 1;
    }

    // Create the tree structure
    std::shared_ptr<Node> tree = CreateTree(root);

    // Display the tree structure
    std::cout << "XML Tree Structure:" << std::endl;
    DisplayTree(tree);

    return 0;
}
