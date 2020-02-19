// Copyright (c) 2017, Tencent Inc.
// All rights reserved.
//
// Author: sevenyou <sevenyou@tencent.com>
// Created: 08/25/17
// Description:

#include "request/object_req.h"
#include "cos_defines.h"
#include "rapidxml/1.13/rapidxml.hpp"
#include "rapidxml/1.13/rapidxml_print.hpp"
#include "rapidxml/1.13/rapidxml_utils.hpp"

namespace qcloud_cos {

bool CompleteMultiUploadReq::GenerateRequestBody(std::string* body) const {
    const std::vector<uint64_t>& part_numbers = GetPartNumbers();
    const std::vector<std::string>& etags = GetEtags();

    // part_numbers与etags长度必须一致
    if (part_numbers.size() != etags.size()) {
        return false;
    }

    // 1.生成CompleteMultiUploadReq需要的xml字符串
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node = doc.allocate_node(rapidxml::node_element,
                                                doc.allocate_string("CompleteMultipartUpload"),
                                                NULL);
    doc.append_node(root_node);

    for (int i = 0; i < part_numbers.size(); ++i) {
        rapidxml::xml_node<>* part_node = doc.allocate_node(rapidxml::node_element,
                                                            doc.allocate_string("Part"),
                                                            NULL);
        std::string etag = StringUtil::StringRemovePrefix(etags[i], "\"");
        etag = StringUtil::StringRemoveSuffix(etag, "\"");
        part_node->append_node(doc.allocate_node(rapidxml::node_element,
                     doc.allocate_string("PartNumber"),
                     doc.allocate_string(StringUtil::Uint64ToString(part_numbers[i]).c_str())));
        part_node->append_node(doc.allocate_node(rapidxml::node_element,
                                         doc.allocate_string("ETag"),
                                         doc.allocate_string(etag.c_str())));
        root_node->append_node(part_node);
    }

    // 2. 填充xml字符串
    rapidxml::print(std::back_inserter(*body), doc, 0);
    doc.clear();

    return true;
}

bool PutObjectACLReq::GenerateRequestBody(std::string* body) const {
    return GenerateAclRequestBody(m_owner, m_acl, body);
}

std::map<std::string, std::string> CopyReq::GetInitHeader() const {
    std::map<std::string, std::string> init_headers;

    std::map<std::string, std::string>::const_iterator c_itr = m_headers_map.begin();
    for (; c_itr != m_headers_map.end(); ++c_itr) {
        if (c_itr->first == "Cache-Control" || c_itr->first == "Content-Disposition"
            || c_itr->first == "Content-Encoding" || c_itr->first == "Content-Type"
            || c_itr->first == "Expires" || c_itr->first == "x-cos-storage-class"
            || c_itr->first == "x-cos-acl" || c_itr->first == "x-cos-grant-read"
            || c_itr->first == "x-cos-grant-write" || c_itr->first == "x-cos-grant-full-control"
            || StringUtil::StringStartsWith(c_itr->first, "x-cos-meta-")) {
            init_headers[c_itr->first] = c_itr->second;
        }
    }

    return init_headers;
}

std::map<std::string, std::string> CopyReq::GetPartCopyHeader() const {
    std::map<std::string, std::string> part_copy_headers;

    std::map<std::string, std::string>::const_iterator c_itr = m_headers_map.begin();
    for (; c_itr != m_headers_map.end(); ++c_itr) {
        if (c_itr->first == "x-cos-copy-source"
            || c_itr->first == "x-cos-copy-source-If-Modified-Since"
            || c_itr->first == "x-cos-copy-source-If-Unmodified-Since"
            || c_itr->first == "x-cos-copy-source-If-Match"
            || c_itr->first == "x-cos-copy-source-If-None-Match") {
            part_copy_headers[c_itr->first] = c_itr->second;
        }
    }

    return part_copy_headers;
}

bool DeleteObjectsReq::GenerateRequestBody(std::string* body) const {
    // 1.生成DeleteObjectsReq需要的xml字符串
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node = doc.allocate_node(rapidxml::node_element,
                                                doc.allocate_string("Delete"),
                                                NULL);
    doc.append_node(root_node);
    root_node->append_node(doc.allocate_node(rapidxml::node_element,
                     doc.allocate_string("Quiet"),
                     doc.allocate_string(m_is_quiet ? "true" : "false")));

    for (std::vector<ObjectVersionPair>::const_iterator c_itr = m_objvers.begin();
            c_itr != m_objvers.end(); ++c_itr) {
        rapidxml::xml_node<>* object_node = doc.allocate_node(rapidxml::node_element,
                doc.allocate_string("Object"), NULL);

        object_node->append_node(doc.allocate_node(rapidxml::node_element,
                    doc.allocate_string("Key"),
                    doc.allocate_string(c_itr->m_object_name.c_str())));

        if (!c_itr->m_version_id.empty()) {
            object_node->append_node(doc.allocate_node(rapidxml::node_element,
                        doc.allocate_string("VersionId"),
                        doc.allocate_string(c_itr->m_version_id.c_str())));
        }
        root_node->append_node(object_node);
    }

    // 2. 填充xml字符串
    rapidxml::print(std::back_inserter(*body), doc, 0);
    doc.clear();

    return true;
}

bool PostObjectRestoreReq::GenerateRequestBody(std::string* body) const {
    // 1.生成PostObjectRestoreReq需要的xml字符串
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node = doc.allocate_node(rapidxml::node_element,
                                                doc.allocate_string("RestoreRequest"),
                                                NULL);
    doc.append_node(root_node);

    root_node->append_node(doc.allocate_node(rapidxml::node_element,
                     doc.allocate_string("Days"),
                     doc.allocate_string(StringUtil::Uint64ToString(m_expiry_days).c_str())));

    rapidxml::xml_node<>* xml_rule = doc.allocate_node(rapidxml::node_element,
            doc.allocate_string("CASJobParameters"), NULL);
    root_node->append_node(xml_rule);
    xml_rule->append_node(doc.allocate_node(rapidxml::node_element, doc.allocate_string("Tier"),
                doc.allocate_string(m_tier.c_str())));

    // 2. 填充xml字符串
    rapidxml::print(std::back_inserter(*body), doc, 0);
    doc.clear();

    return true;
}

void SelectObjectContentReq::SetExpression(const std::string& expression, ExpressionType type = SQL) {
    this->m_expression = expression;
    this->m_expression_type = type;
}

void SelectObjectContentReq::SetInputFormat(InputFormat* inputformat) {
    this->m_inputformat = inputformat;
}

void SelectObjectContentReq::SetOutputFormat(OutputFormat* outputformat) {
    this->m_outputformat = outputformat;
}

void SelectObjectContentReq::SetRequetProgress(bool requestProgress) {
    this->m_request_progress = requestProgress;
}

ExpressionType SelectObjectContentReq::GetExpressionType() const {
    return this->m_expression_type;
}

std::string SelectObjectContentReq::GetExpression() const {
    return this->m_expression;
}

InputFormat* SelectObjectContentReq::GetInputFormat() const {
    return this->m_inputformat;
}

OutputFormat* SelectObjectContentReq::GetOutputFormat() const {
    return this->m_outputformat;
}

bool SelectObjectContentReq::GetRequetProgress() const {
    return this->m_request_progress;
}

bool SelectObjectContentReq::GenerateRequestBody(std::string* body) const {
    // 1.生成SelectObjectContentReq需要的xml字符串
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node = doc.allocate_node(rapidxml::node_element,
                                                doc.allocate_string("SelectRequest"),
                                                NULL);
    doc.append_node(root_node);

    root_node->append_node(doc.allocate_node(rapidxml::node_element,
                     doc.allocate_string("Expression"),
                     doc.allocate_string(GetExpression().c_str())));

    root_node->append_node(doc.allocate_node(rapidxml::node_element,
                     doc.allocate_string("ExpressionType"), 
                     doc.allocate_string(ExpressionType_Str(GetExpressionType()).c_str())));

    InputFormat* inputformat = GetInputFormat();
    rapidxml::xml_node<>* input_node;
    inputformat->toXMLNode(input_node, &doc, root_node, 0);
      
    OutputFormat* outputformat = GetOutputFormat();
    rapidxml::xml_node<>* output_node;
    outputformat->toXMLNode(output_node, &doc, root_node);

    rapidxml::xml_node<>* request_progress_node = doc.allocate_node(rapidxml::node_element,
                                                doc.allocate_string("RequestProgress"),
                                                NULL);

    request_progress_node->append_node(doc.allocate_node(rapidxml::node_element,
                     doc.allocate_string("Enabled"), 
                     doc.allocate_string(Bool_Str(GetRequetProgress()).c_str())));

    root_node->append_node(request_progress_node);

    // 2. 填充xml字符串
    rapidxml::print(std::back_inserter(*body), doc, 0);
    doc.clear();

    return true;
}

} // namespace qcloud_cos
