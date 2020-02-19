#include "rapidxml/1.13/rapidxml.hpp"
#include "rapidxml/1.13/rapidxml_print.hpp"
#include "rapidxml/1.13/rapidxml_utils.hpp"

#include "cos_defines.h"

namespace qcloud_cos {

std::string CSVHeader_Str(CSVHeader num) {
    const std::string strings[] = {
        "NONE\0", "IGNORE\0", "USE\0"
    };
    return strings[num];
}

std::string CompressionType_Str(CompressionType num) {
    const std::string strings[] = {
        "NONE\0",
        "GZIP\0"
    };
    return strings[num];
}

std::string Bool_Str(bool flag){
    if (flag) {
        return "TRUE";
    } else {
        return "FALSE";
    }
}

std::string JsonType_Str(JsonType num){
    const std::string strings[] = {
        "DOCUMENT\0",
        "LINES\0"
    };
    return strings[num];
}

std::string ExpressionType_Str(ExpressionType type){
    const std::string strings[] = {
        "SQL\0",
    };
    return strings[type];
}


InputFormat::InputFormat():
    m_compression_type(NONE) {
}

InputFormat::InputFormat(CompressionType compressionType):
    m_compression_type(compressionType) {
}

void InputFormat::SetCompressionType(CompressionType compressionType) {
    this->m_compression_type = compressionType;
}

CompressionType InputFormat::GetCompressionType() {
    return this->m_compression_type;
}

CSVInputFormat::CSVInputFormat():InputFormat(NONE),
    m_headerInfo(None), m_record_delimiter("\\n"),
    m_field_delimiter(","), m_quote_character("\""),
    m_quote_escape_character("\""), m_comments("#"), 
    m_allow_quote_record_delimiter(false) {
}

CSVInputFormat::CSVInputFormat(CompressionType compression_type, CSVHeader headerInfo,
                               const std::string& record_delimiter,
                               const std::string& field_delimiter, const std::string& quote_character,
                               const std::string& quote_escape_character, const std::string& comments, 
                               bool allow_quoted_record_delimiter):
    InputFormat(compression_type), m_headerInfo(headerInfo), 
    m_record_delimiter(record_delimiter), m_field_delimiter(field_delimiter), 
    m_quote_character(quote_character), m_quote_escape_character(quote_escape_character),
    m_comments(comments), 
    m_allow_quote_record_delimiter(allow_quoted_record_delimiter) {
     
}

void CSVInputFormat::SetHeaderInfo(CSVHeader headerInfo) {
    this->m_headerInfo = headerInfo;
}

void CSVInputFormat::SetRecordDelimiter(const std::string& record_delimiter) {
    this->m_record_delimiter = record_delimiter;
}

void CSVInputFormat::SetFieldDelimiter(const std::string& field_delimiter) {
    this->m_field_delimiter = field_delimiter;
}

void CSVInputFormat::SetQuoteCharacter(const std::string& quote_character) {
    this->m_quote_character = quote_character;
}

void CSVInputFormat::SetQuoteEscapeCharacter(const std::string& quote_escape_character) {
    this->m_quote_escape_character = quote_escape_character;
}

void CSVInputFormat::SetComments(const std::string& comments) {
    this->m_comments = comments;
}

void CSVInputFormat::SetAllowQuoteRecordDelimiter(bool allow_quote_record_delimiter) {
    this->m_allow_quote_record_delimiter = allow_quote_record_delimiter;
}

CSVHeader CSVInputFormat::GetHeaderInfo() const {
    return this->m_headerInfo;
}

const std::string& CSVInputFormat::GetRecordDelimiter() const {
    return this->m_record_delimiter;
}

const std::string& CSVInputFormat::GetFieldDelimiter() const {
    return this->m_field_delimiter;
}

const std::string& CSVInputFormat::GetQuoteCharacter() const {
    return this->m_quote_character;
}

const std::string& CSVInputFormat::GetQuoteEscapeCharacter() const {
    return this->m_quote_escape_character;
}

const std::string& CSVInputFormat::GetComments() const {
    return this->m_comments;
}
bool CSVInputFormat::GetAllowQuoteRecordDelimiter() const {
    return this->m_allow_quote_record_delimiter;
}

void CSVInputFormat::toXMLNode(rapidxml::xml_node<>* node,rapidxml::xml_document<> *doc, 
                               rapidxml::xml_node<>* root_node, int flag) {
    
    node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("InputSerialization"),
                             NULL);
        
    node->append_node(doc->allocate_node(rapidxml::node_element, "CompressionType",
                             doc->allocate_string(CompressionType_Str(GetCompressionType()).c_str())));

    rapidxml::xml_node<>* csv_node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("CSV"), NULL);

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "FileHeaderInfo",
                             doc->allocate_string(CSVHeader_Str(GetHeaderInfo()).c_str())));

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "RecordDelimiter",
                             doc->allocate_string(GetRecordDelimiter().c_str())));

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "FieldDelimiter",
                             doc->allocate_string(GetFieldDelimiter().c_str())));

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "QuoteCharacter",
                             doc->allocate_string(GetQuoteCharacter().c_str()))); 

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "QuoteEscapeCharacter",
                             doc->allocate_string(GetQuoteEscapeCharacter().c_str())));  

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "Comments",
                             doc->allocate_string(GetComments().c_str()))); 

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "AllowQuotedRecordDelimiter",
                             doc->allocate_string(Bool_Str(GetAllowQuoteRecordDelimiter()).c_str())));

    node->append_node(csv_node);
    
    root_node->append_node(node);
}

JSONInputFormat::JSONInputFormat():
    InputFormat(NONE), m_jsontype(DOCUMENT)  {
}

JSONInputFormat::JSONInputFormat(JsonType jsonType):
    InputFormat(NONE), m_jsontype(jsonType)  {
}

void JSONInputFormat::SetJsonType(JsonType jsonType) {
    this->m_jsontype = jsonType;
}

JsonType JSONInputFormat::GetJsonInfo() const {
    return this->m_jsontype;
}

void JSONInputFormat::toXMLNode(rapidxml::xml_node<>* node, rapidxml::xml_document<> *doc,
                                rapidxml::xml_node<>* root_node, int flag) {

    node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("InputSerialization"),
                             NULL);
    node->append_node(doc->allocate_node(rapidxml::node_element, "CompressionType",
                             doc->allocate_string(CompressionType_Str(GetCompressionType()).c_str())));

    rapidxml::xml_node<>* json_node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("JSON"), NULL);

    json_node->append_node(doc->allocate_node(rapidxml::node_element, "Type",
                             doc->allocate_string(JsonType_Str(GetJsonInfo()).c_str())));

    node->append_node(json_node);
    root_node->append_node(node);
}


OutputFormat::OutputFormat():
    m_record_delimiter("\\n") {
}

OutputFormat::OutputFormat(const std::string& record_delimiter):
    m_record_delimiter(record_delimiter) {
}

void OutputFormat::SetRecordDelimiter(std::string& record_delimiter) {
    this->m_record_delimiter = record_delimiter;
}

std::string OutputFormat::GetRecordDelimiter() {
    return this->m_record_delimiter;
}

CSVOutputFormat::CSVOutputFormat():OutputFormat("\\n"),
    m_quote_fields("ASNEEDED"), m_field_delimiter(","),
    m_quote_character("\""), 
    m_quote_escape_character("\"") {
}

CSVOutputFormat::CSVOutputFormat(const std::string& record_delimiter,
            const std::string& quote_fields, const std::string& field_delimiter,
            const std::string& quote_character, const std::string& quote_escape_character):
    OutputFormat(record_delimiter), m_quote_fields(quote_fields), 
    m_field_delimiter(field_delimiter), m_quote_character(quote_character), 
    m_quote_escape_character(quote_escape_character) {
    
}

void CSVOutputFormat::SetQuoteFields(const std::string& quote_fields) {
    this->m_quote_fields = quote_fields;
}

void CSVOutputFormat::SetFieldDelimiter(const std::string& field_delimiter) {
    this->m_field_delimiter = field_delimiter;
}

void CSVOutputFormat::SetQuoteCharacter(const std::string& quote_character) {
    this->m_quote_character = quote_character;
}

void CSVOutputFormat::SetQuoteEscapeCharacter(const std::string& quote_escape_character) {
    this->m_quote_escape_character = quote_escape_character;
}

std::string CSVOutputFormat::GetQuoteFields() const {
    return this->m_quote_fields;
}

std::string CSVOutputFormat::GetFieldDelimiter() const {
    return this->m_field_delimiter;
}

std::string CSVOutputFormat::GetQuoteCharacter() const {
    return this->m_quote_character;
}

std::string CSVOutputFormat::GetQuoteEscapeCharacter() const {
    return this->m_quote_escape_character;
}

void CSVOutputFormat::toXMLNode(rapidxml::xml_node<>* node, rapidxml::xml_document<>* doc, 
                                rapidxml::xml_node<>* root_node) {  
  
    node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("OutputSerialization"),
                             NULL);

    rapidxml::xml_node<>* csv_node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("CSV"), NULL);

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "QuoteFields",
                             doc->allocate_string(GetQuoteFields().c_str())));

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "RecordDelimiter",
                             doc->allocate_string(GetRecordDelimiter().c_str())));

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "FieldDelimiter",
                             doc->allocate_string(GetFieldDelimiter().c_str())));

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "QuoteCharacter",
                             doc->allocate_string(GetQuoteCharacter().c_str()))); 

    csv_node->append_node(doc->allocate_node(rapidxml::node_element, "QuoteEscapeCharacter",
                             doc->allocate_string(GetQuoteEscapeCharacter().c_str())));  

    node->append_node(csv_node);
    root_node->append_node(node);
}

JSONOutputFormat::JSONOutputFormat():OutputFormat("\\n") {

}

JSONOutputFormat::JSONOutputFormat(const std::string& recordDelimiter):
                                  OutputFormat(recordDelimiter) {
}

void JSONOutputFormat::toXMLNode(rapidxml::xml_node<>* node, rapidxml::xml_document<> *doc,
                                  rapidxml::xml_node<>* root_node) {
    node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("OutputSerialization"),
                             NULL);
    rapidxml::xml_node<>* json_node = doc->allocate_node(rapidxml::node_element,
                             doc->allocate_string("JSON"), NULL);

    json_node->append_node(doc->allocate_node(rapidxml::node_element, "RecordDelimiter",
                             doc->allocate_string(GetRecordDelimiter().c_str()))); 
    node->append_node(json_node);
    root_node->append_node(node);                                                 
}

} // namespace qcloud_cos
